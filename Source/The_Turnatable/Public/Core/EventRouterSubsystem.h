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
};

// C++ multicast delegate for event delivery.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventMessage, const FEventMessage&);

UCLASS()
class THE_TURNATABLE_API UEventRouterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:	
	template <typename TPayloadStruct>
	static bool BroadcastEvent(UObject* Sender, const FName TopicName, const TPayloadStruct& Message)
	{
		if (!Sender) return false;

		UWorld* World = Sender->GetWorld();
		if (!World) return false;

		UEventRouterSubsystem* Router = GetEventRouterSubsystem(World);
		if (!Router) return false;

		const FGameplayTag Topic = FGameplayTag::RequestGameplayTag(TopicName, false);
		if (!Topic.IsValid()) return false;

		Router->PublishTyped<TPayloadStruct>(Topic, Sender, Message);
		return true;
	}

	template <typename TPayloadStruct, typename TObject>
	static FDelegateHandle SubscribeToEvent(TObject* Listener, const FName TopicName, void (TObject::*Method)(const TPayloadStruct&))
	{
		static_assert(TIsDerivedFrom<TObject, UObject>::IsDerived, "Listener must be a UObject type.");

		if (!Listener || !Method) return FDelegateHandle();

		UWorld* World = Listener->GetWorld();
		if (!World) return FDelegateHandle();

		UEventRouterSubsystem* Router = GetEventRouterSubsystem(World);
		if (!Router) return FDelegateHandle();

		const FGameplayTag Topic = FGameplayTag::RequestGameplayTag(TopicName, false);
		if (!Topic.IsValid()) return FDelegateHandle();

		return Router->SubscribeTyped<TPayloadStruct>(Topic, Listener, Method);
	}

	template <typename TObject>
	static bool UnsubscribeFromEvent(TObject* Listener, const FName Topic, FDelegateHandle& Handle)
	{
		static_assert(TIsDerivedFrom<TObject, UObject>::IsDerived, "Listener must be a UObject type.");

		if (!Listener) return false;
		if (!Handle.IsValid()) return false;

		UWorld* World = Listener->GetWorld();
		if (!World) return false;

		if (UEventRouterSubsystem* Router = GetEventRouterSubsystem(World))
		{
			const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(Topic, false);
			if (!Tag.IsValid()) return false;

			Router->Unsubscribe(Tag, Handle);

			// Opcional: invalida el handle para evitar doble-unsubscribe accidental
			Handle.Reset();

			return true;
		}

		return false;
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
	
private:
	// Listeners keyed by the topic they subscribed to.
	TMap<FGameplayTag, FOnEventMessage> TopicDelegates;
	
	static UEventRouterSubsystem* GetEventRouterSubsystem(UWorld* World);;
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