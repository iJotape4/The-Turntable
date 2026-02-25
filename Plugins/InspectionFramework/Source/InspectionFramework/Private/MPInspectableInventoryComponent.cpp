// Fill out your copyright notice in the Description page of Project Settings.
#include "MPInspectableInventoryComponent.h"

#include "MPInspectItem.h"
#include "UI/MPInventorySlot.h"

void UMPInspectableInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InspectItemActor = GetWorld()->SpawnActor<AMPInspectItem>(InspectItemClass);
	InspectItemActor->SetActorLocation (FVector(1000.0f,0.0f,1000.0f) );	
}

void UMPInspectableInventoryComponent::ToggleInventory()
{
	if (!InspectItemActor) return;
	if (InspectItemActor->IsInspecting())
	{
		CloseInspectView();
	}
	else
	{
		Super::ToggleInventory();
	}
}

void UMPInspectableInventoryComponent::SlotSelected(const FSlotSelectedEvent& SlotSelectedEvent)
{
	if (InventoryMode == EInventoryMode::Default)
	{
		InspectItemActor->InspectItem(SlotSelectedEvent.InventorySlot->GetItem());
	}
	else
	{
		Super::SlotSelected(SlotSelectedEvent);
	}
}

void UMPInspectableInventoryComponent::RotateItem(const FVector2D AxisValue) const
{
	if (!InspectItemActor->IsInspecting())
		return;
	InspectItemActor->RotateItem(AxisValue *RotationSpeed);
}

void UMPInspectableInventoryComponent::OnPointerDown()
{
	if (!InspectItemActor->IsInspecting())
		return;
	InspectItemActor->CheckHitResult();
}
void UMPInspectableInventoryComponent::CloseInspectView()
{
	InspectItemActor->CloseInspectWidget();
}

void UMPInspectableInventoryComponent::OnZoom(const float Value) const
{
	if (!InspectItemActor->IsInspecting())
		return;
	InspectItemActor->Zoom(Value);
}