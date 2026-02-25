// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
#include "MPKeyItemDropZone.h"

#include "EventRouterSubsystem.h"
#include "GenericPayloads.h"
#include "MPItem.h"
#include "Components/SphereComponent.h"

AMPKeyItemDropZone::AMPKeyItemDropZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DropZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("DropZoneMesh");
	RootComponent = DropZoneMesh;

	SphereComponent->SetupAttachment(DropZoneMesh);
}

bool AMPKeyItemDropZone::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bAcceptsItems) return false;
	return Super::Interact_Implementation(InstigatorPawn);
}

bool  AMPKeyItemDropZone::IsItemRequired(UMPItem* Item)
{
	if (Item == nullptr) return false;
	if (RequiredItems.Contains(Item)) return true;

	return false;
}

bool AMPKeyItemDropZone::ReceiveItem(const UMPItem* Item)
{
	if (Item == nullptr) return false;

	if (RequiredItems.Num() >= 0)
	{
		for (UMPItem* RequiredItem : RequiredItems)
		{
			if (Item == RequiredItem )
			{
				AcceptItem(RequiredItem);
				RequiredItems.Remove(RequiredItem);

				if (RequiredItems.Num()<=0)
				{
					bAcceptsItems = false;
					SphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

					//TODO: Localization System
					UEventRouterSubsystem::BroadcastEvent(this, UIDialogEventsTag, FGenericUIDialogueEvent{FText::FromString("Well done! GG WP")} );
					FinishPuzzle();
				}
				else
				{
					//TODO: Localization System
					UEventRouterSubsystem::BroadcastEvent(this, UIDialogEventsTag, FGenericUIDialogueEvent{RequiredItem->DropOffSentence});
				}
				return true;
			}
			else
			{
				//TODO: Localization System
				UEventRouterSubsystem::BroadcastEvent(this, UIDialogEventsTag, FGenericUIDialogueEvent{FText::FromString("This item doesn't seem to fit here.")});
			}
		}
	}
	//else
	return false;
}

void AMPKeyItemDropZone::DispatchEvent_Implementation(const FInstancedStruct& Payload)
{
	UEventRouterSubsystem::BroadcastEvent(this, EventTopic, Payload);
}