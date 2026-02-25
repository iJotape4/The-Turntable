// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MPInventorySlot.h"

#include "EventRouterSubsystem.h"
#include "MPInventoryComponent.h"
#include "MPItem.h"

void UMPInventorySlot::Onclick()
{
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT ("Item: %s"), *Item->ItemName.ToString());
		UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", FSlotSelectedEvent{this});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT ("No Item"));
	}
}

void UMPInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	APawn* OwningPawn = GetOwningPlayerPawn();
	if (!OwningPawn && GetWorld())
	{
		OwningPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	}

	if (OwningPawn)
	{
		InventoryComponent = OwningPawn->FindComponentByClass<UMPInventoryComponent>();
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

UMPItem* UMPInventorySlot::GetItem() const
{
	return Item;
}