#include "EventRouterSubsystem.h"

FDelegateHandle UEventRouterSubsystem::Subscribe(const FGameplayTag Topic, FOnEventMessage::FDelegate&& Delegate)
{
	if (!Topic.IsValid())
	{
		return FDelegateHandle();
	}

	FOnEventMessage& Multicast = TopicDelegates.FindOrAdd(Topic);
	return Multicast.Add(MoveTemp(Delegate));
}

void UEventRouterSubsystem::Unsubscribe(const FGameplayTag Topic, const FDelegateHandle Handle)
{
	if (!Handle.IsValid())
	{
		return;
	}

	if (FOnEventMessage* Multicast = TopicDelegates.Find(Topic))
	{
		Multicast->Remove(Handle);

		// Optional cleanup: remove empty entries.
		if (!Multicast->IsBound())
		{
			TopicDelegates.Remove(Topic);
		}
	}
}

void UEventRouterSubsystem::Publish(const FEventMessage& Message)
{
	if (!Message.Topic.IsValid())
	{
		return;
	}

	// Dispatch to all subscriptions where:
	// PublishedTopic.MatchesTag(SubscribedTopic) is true
	// e.g. publish UI.Inventory -> subscribers of UI and UI.Inventory receive it.
	for (TPair<FGameplayTag, FOnEventMessage>& Pair : TopicDelegates)
	{
		const FGameplayTag& SubscribedTopic = Pair.Key;

		if (Message.Topic.MatchesTag(SubscribedTopic))
		{
			Pair.Value.Broadcast(Message);
		}
	}
}

void UEventRouterSubsystem::PublishInstanced(const FGameplayTag Topic, UObject* Sender, const FInstancedStruct& Payload)
{
	Publish(FEventMessage::Make(Topic, Sender, Payload));
}

UEventRouterSubsystem* UEventRouterSubsystem::GetEventRouterSubsystem(UWorld* World)
{
	if (World == nullptr)
	{
		return nullptr;
	}

	if (UGameInstance* GI = World->GetGameInstance())
	{
		return GI->GetSubsystem<UEventRouterSubsystem>();
	}
	
	return nullptr;
}
