#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h" // FInstancedStruct (add StructUtils to Build.cs)
#include "EventRouterSubsystem.generated.h"

/**
 * Generic event message (topic + payload + optional sender).
 */
USTRUCT(BlueprintType)
struct FEventMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag Topic;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> Sender = nullptr;

	// Type-erased payload (can hold any UScriptStruct value).
	UPROPERTY()
	FInstancedStruct Payload;

	FEventMessage() = default;

	template <typename TPayloadStruct>
	static FEventMessage Make(const FGameplayTag InTopic, UObject* InSender, const TPayloadStruct& InPayload)
	{
		FEventMessage Msg;
		Msg.Topic = InTopic;
		Msg.Sender = InSender;
		Msg.Payload.InitializeAs<TPayloadStruct>(InPayload);
		return Msg;
	}

	static FEventMessage Make(const FGameplayTag InTopic, UObject* InSender, const FInstancedStruct& InPayload)
	{
		FEventMessage Msg;
		Msg.Topic  = InTopic;
		Msg.Sender = InSender;
		Msg.Payload = InPayload; // no InitializeAs here
		return Msg;
	}
};

// C++ multicast delegate for event delivery.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventMessage, const FEventMessage&);

UCLASS()
class EVENTROUTER_API UEventRouterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:	
	template <typename TPayloadStruct>
		static bool BroadcastEvent(UObject* Sender, const FGameplayTag Topic, const TPayloadStruct& Message)
	{
		if (UEventRouterSubsystem* Router = GetValidatedRouter(Sender, Topic))
		{
			Router->PublishTyped<TPayloadStruct>(Topic, Sender, Message);
			return true;
		}
		return false;
	}

	template <typename TPayloadStruct>
	static bool BroadcastEvent(UObject* Sender, const FName TopicName, const TPayloadStruct& Message)
	{
		return BroadcastEvent(Sender, FGameplayTag::RequestGameplayTag(TopicName, false), Message);
	}

	static bool BroadcastEvent(UObject* Sender, const FGameplayTag Topic, const FInstancedStruct& Payload)
	{
		if (UEventRouterSubsystem* Router = GetValidatedRouter(Sender, Topic))
		{
			Router->PublishInstanced(Topic, Sender, Payload);
			return true;
		}
		return false;
	}

	static bool BroadcastEvent(UObject* Sender, const FName TopicName, const FInstancedStruct& Payload)
	{
		return BroadcastEvent(Sender, FGameplayTag::RequestGameplayTag(TopicName, false), Payload);
	}


	template <typename TPayloadStruct, typename TObject>
	static FDelegateHandle SubscribeToEvent(TObject* Listener, const FGameplayTag Topic, void (TObject::*Method)(const TPayloadStruct&))
	{
		static_assert(TIsDerivedFrom<TObject, UObject>::IsDerived, "Listener must be a UObject type.");

		if (!Listener || !Method) return FDelegateHandle();

		UWorld* World = Listener->GetWorld();
		if (!World) return FDelegateHandle();

		UEventRouterSubsystem* Router = GetEventRouterSubsystem(World);
		if (!Router) return FDelegateHandle();

		if (!Topic.IsValid()) return FDelegateHandle();

		return Router->SubscribeTyped<TPayloadStruct>(Topic, Listener, Method);
	}

	template <typename TPayloadStruct, typename TObject>
	static FDelegateHandle SubscribeToEvent(TObject* Listener, const FName Topic, void (TObject::*Method)(const TPayloadStruct&))
	{
		return SubscribeToEvent(Listener, FGameplayTag::RequestGameplayTag(Topic, false), Method);
	}
	
	template <typename TObject>
	static bool UnsubscribeFromEvent(TObject* Listener, const FGameplayTag Topic, FDelegateHandle& Handle)
	{
		static_assert(TIsDerivedFrom<TObject, UObject>::IsDerived, "Listener must be a UObject type.");

		if (!Listener) return false;
		if (!Handle.IsValid()) return false;

		UWorld* World = Listener->GetWorld();
		if (!World) return false;

		if (UEventRouterSubsystem* Router = GetEventRouterSubsystem(World))
		{
			if (!Topic.IsValid()) return false;
			Router->Unsubscribe(Topic, Handle);
			//Optional: Invalidates the handle to avoid an unsubscribe twice accidentally
			Handle.Reset();

			return true;
		}
		return false;
	}

	template <typename TObject>
	static bool UnsubscribeFromEvent(TObject* Listener, const FName Topic, FDelegateHandle& Handle)
	{
		return UnsubscribeFromEvent(Listener, FGameplayTag::RequestGameplayTag(Topic, false), Handle);
	}
	
protected:
	// Subscribe with a raw delegate (lambda, static, etc.). Returns a handle you can store.
	FDelegateHandle Subscribe(const FGameplayTag Topic, FOnEventMessage::FDelegate&& Delegate);

	// Convenience: subscribe a UObject member function that takes a typed payload.
	template <typename TPayloadStruct, typename TObject>
	FDelegateHandle SubscribeTyped(const FGameplayTag Topic, TObject* Listener, void (TObject::*Method)(const TPayloadStruct&));

	// Unsubscribe using the original topic + handle.
	void Unsubscribe(const FGameplayTag Topic, const FDelegateHandle Handle);

	// Publish with a prebuilt message.
	void Publish(const FEventMessage& Message);

	// Convenience: publish a typed payload.
	template <typename TPayloadStruct>
	void PublishTyped(const FGameplayTag Topic, UObject* Sender, const TPayloadStruct& Payload);
	
	void PublishInstanced(const FGameplayTag Topic, UObject* Sender, const FInstancedStruct& Payload);
	
private:
	// Listeners keyed by the topic they subscribed to.
	TMap<FGameplayTag, FOnEventMessage> TopicDelegates;
	
	static UEventRouterSubsystem* GetEventRouterSubsystem(UWorld* World);;

	static UEventRouterSubsystem* GetValidatedRouter(UObject* Sender, const FGameplayTag& Topic)
	{
		if (!Sender || !Topic.IsValid()) return nullptr;
		UWorld* World = Sender->GetWorld();
		return World ? GetEventRouterSubsystem(World) : nullptr;
	}
};

template <typename TPayloadStruct, typename TObject>
FDelegateHandle UEventRouterSubsystem::SubscribeTyped(const FGameplayTag Topic, TObject* Listener, void (TObject::*Method)(const TPayloadStruct&))
{
	static_assert(TIsDerivedFrom<TObject, UObject>::IsDerived, "Listener must be a UObject type.");

	// Bind to the generic message, then extract the typed payload.
	// Use CreateWeakLambda to ensure the execution is safe and stops if Listener is destroyed.
	FOnEventMessage::FDelegate Delegate = FOnEventMessage::FDelegate::CreateWeakLambda(
		Listener,
		[Listener, Method](const FEventMessage& Msg)
		{
			// Ensure the payload is the expected type.
			const TPayloadStruct* Payload = Msg.Payload.GetPtr<TPayloadStruct>();
			if (Payload)
			{
				(Listener->*Method)(*Payload);
			}
		}
	);

	return Subscribe(Topic, MoveTemp(Delegate));
}

template <typename TPayloadStruct>
void UEventRouterSubsystem::PublishTyped(const FGameplayTag Topic, UObject* Sender, const TPayloadStruct& Payload)
{
	Publish(FEventMessage::Make<TPayloadStruct>(Topic, Sender, Payload));
}