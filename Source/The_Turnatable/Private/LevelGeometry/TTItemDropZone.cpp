// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelGeometry/TTItemDropZone.h"

#include "Components/SphereComponent.h"
#include "Core/EventPayloads/LevelProgressPayloads.h"
#include "Core/Inventory/TTItem.h"

// Sets default values
ATTItemDropZone::ATTItemDropZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DropZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("DropZoneMesh");
	RootComponent = DropZoneMesh;

	SphereComponent->SetupAttachment(DropZoneMesh);
}

bool ATTItemDropZone::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bAcceptsItems) return false;
	return Super::Interact_Implementation(InstigatorPawn);
}

bool  ATTItemDropZone::IsItemRequired(UTTItem* Item)
{
	if (Item == nullptr) return false;
	if (RequiredItems.Contains(Item)) return true;

	return false;
}

bool ATTItemDropZone::ReceiveItem(UTTItem* Item)
{
	if (Item == nullptr) return false;

	if (RequiredItems.Num() >= 0)
	{
		for (UTTItem* RequiredItem : RequiredItems)
		{
			if (Item == RequiredItem )
			{
				AcceptItem(RequiredItem);
				UEventRouterSubsystem::BroadcastEvent(this, "UI.Dialogues", FGenericDialogueEvent{RequiredItem->DropOffSentence});
				RequiredItems.Remove(RequiredItem);

				if (RequiredItems.Num()<=0)
				{
					bAcceptsItems = false;
					SphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
					FinishPuzzle();
				}
				return true;
			}
			else
			{
				UEventRouterSubsystem::BroadcastEvent(this, "UI.Dialogues", FGenericDialogueEvent{FText::FromString("This item doesn't seem to fit here.")});
			}
		}
	}
	//else
	return false;
}

void ATTItemDropZone::DispatchEvent_Implementation(const FInstancedStruct& Payload)
{
	UEventRouterSubsystem::BroadcastEvent(this, EventTopic, Payload);
}