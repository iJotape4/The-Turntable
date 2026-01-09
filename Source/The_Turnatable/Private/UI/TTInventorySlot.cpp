// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInventorySlot.h"

#include "Core/EventRouterSubsystem.h"
#include "Core/Inventory/TTInventoryComponent.h"
#include "Core/Inventory/TTItem.h"
#include "LevelGeometry/TTInspectItem.h"

void UTTInventorySlot::Onclick()
{
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT ("Item: %s"), *Item->ItemName.ToString());
		InventoryComponent->InspectItemActor->InspectItem(Item);
		//UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", FSlotSelectedEvent{this});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT ("No Item"));
	}
}

void UTTInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn && GetWorld())
	{
		OwningPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	}

	if (OwningPawn)
	{
		InventoryComponent = OwningPawn->FindComponentByClass<UTTInventoryComponent>();
		if (!InventoryComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent not found on pawn %s"), *OwningPawn->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwningPawn not found in UTTInventorySlot::NativeConstruct"));
	}
}

UTTItem* UTTInventorySlot::GetItem() const
{
	return Item;
}
