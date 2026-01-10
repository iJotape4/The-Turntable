// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Inventory/TTInventoryComponent.h"
#include "InputActionValue.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/EventPayloads/InventoryEventPayloads.h"
#include "Core/Inventory/TTItem.h"
#include "LevelGeometry/TTInspectItem.h"
#include "LevelGeometry/TTItemDropZone.h"
#include "UI/TTInventorySlot.h"

struct FInputActionValue;

// Sets default values for this component's properties
UTTInventoryComponent::UTTInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTTInventoryComponent::PostInitProperties()
{
	Super::PostInitProperties();
	
}

void UTTInventoryComponent::MatchKeyItemEvent(const FMatchKeyItemEvent& MatchKeyItemEvent)
{
	if (HasItem(MatchKeyItemEvent.DropZone->RequiredItem))
	{
		InventoryMode = EInventoryMode::MatchKeyItem;
		CurrentDropZone = MatchKeyItemEvent.DropZone;
		ToggleInventory();
		UE_LOG(LogTemp, Warning, TEXT("Toggled inventroy in mode %hhd"), InventoryMode);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player does not have required item to match"));
	}
}

void UTTInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InspectItemActor = GetWorld()->SpawnActor<ATTInspectItem>(InspectItemClass);

	
	InventoryPickedUpHandle = UEventRouterSubsystem::SubscribeToEvent<FItemPickedEvent>(
		this, 
		UIEventsTag, 
		&UTTInventoryComponent::AddItem // Use '&' and the full class name
	);

	
	InventoryToggleHandle = UEventRouterSubsystem::SubscribeToEvent<FInventoryToggle>(this, UIEventsTag, &UTTInventoryComponent::ToggleInventory);
	InventoryMatchItemHandle = UEventRouterSubsystem::SubscribeToEvent<FMatchKeyItemEvent>(this, UIEventsTag, &UTTInventoryComponent::MatchKeyItemEvent);
	InventorySlotSelectedHandle = UEventRouterSubsystem::SubscribeToEvent<FSlotSelectedEvent>(this,UIEventsTag,&UTTInventoryComponent::SlotSelected);
	InventoryItemDroppedHandle = UEventRouterSubsystem::SubscribeToEvent<FItemDroppedEvent>(this,UIEventsTag,&UTTInventoryComponent::RemoveItem);

}

void UTTInventoryComponent::AddItem(const FItemPickedEvent& Ev)
{
	Inventory.Add(Ev.Item);
}

void UTTInventoryComponent::RemoveItem(const FItemDroppedEvent& Event)
{
	RemoveItem(Event.InventorySlot->GetItem());
}

void UTTInventoryComponent::RemoveItem(UTTItem* ItemToRemove)
{
	Inventory.Remove(ItemToRemove);
}

bool UTTInventoryComponent::HasItem(UTTItem* ItemToCheck) const
{
	return Inventory.Contains(ItemToCheck);
}

void UTTInventoryComponent::ToggleInventory()
{
	if (!InspectItemActor) return;
	if (InspectItemActor->IsInspecting())
	{
		CloseInspectView();
	}
	else
	{
		bInventoryOpen = !bInventoryOpen;
		UEventRouterSubsystem::BroadcastEvent(this, UIEventsTag, FInventoryToggle(bInventoryOpen));
	}
}

void UTTInventoryComponent::ToggleInventory(const FInventoryToggle& Event)
{
	bInventoryOpen = Event.bOpen;
}

void UTTInventoryComponent::SlotSelected(const FSlotSelectedEvent& SlotSelectedEvent)
{
	if (InventoryMode == EInventoryMode::Default)
	{
		InspectItemActor->InspectItem(SlotSelectedEvent.InventorySlot->GetItem());
	}
	else if (InventoryMode == EInventoryMode::MatchKeyItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Picking in TryDropItem:"));
		if ( CurrentDropZone->ReceiveItem(SlotSelectedEvent.InventorySlot->GetItem()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item dropped"));
			UEventRouterSubsystem::BroadcastEvent(this, UIEventsTag, FItemDroppedEvent{SlotSelectedEvent.InventorySlot});
			ToggleInventory();
		}
	}
}


void UTTInventoryComponent::RotateItem(const FInputActionValue& Value)
{
	InspectItemActor->RotateItem(Value.Get<FVector2D>()*3.0f);
}

void UTTInventoryComponent::CloseInspectView()
{
	InspectItemActor->CloseInspectWidget();
}

void UTTInventoryComponent::BeginDestroy()
{
	UEventRouterSubsystem::UnsubscribeFromEvent(this, "UI.Inventory", InventoryPickedUpHandle);
	Super::BeginDestroy();
}