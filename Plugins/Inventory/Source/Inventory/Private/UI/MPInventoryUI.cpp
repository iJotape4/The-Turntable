// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MPInventoryUI.h"

#include "EventRouterSubsystem.h"
#include "MPInventoryEventPayloads.h"
#include "MPItem.h"
#include "Components/PanelWidget.h"
#include "UI/MPInventorySlot.h"
#include "UMPBackKeyWidget.h"

void UMPInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryPickedUpItemHandle = UEventRouterSubsystem::SubscribeToEvent<FItemPickedEvent>(this, InventoryUIEventsTag, &UMPInventoryUI::OnAddItem);
	InventoryToggleHandle = UEventRouterSubsystem::SubscribeToEvent<FInventoryToggle>(this, InventoryUIEventsTag, &UMPInventoryUI::ToggleInventory);
	InventoryItemDroppedHandle = UEventRouterSubsystem::SubscribeToEvent<FItemDroppedEvent>(this,InventoryUIEventsTag,&UMPInventoryUI::OnRemoveItem);
	InventoryItemUpdatedHandle = UEventRouterSubsystem::SubscribeToEvent<FItemUpdatedEvent>(this,InventoryUIEventsTag,&UMPInventoryUI::OnUpdateItem);

	
	UE_LOG(LogTemp, Warning, TEXT("Constructed Inventory UI with BackKeyWidgetName: %s"), *BackKeyWidgetName.ToString());
	BackKeyWidget = Cast<UMPBackKeyWidget>(GetWidgetFromName(BackKeyWidgetName));
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.AddDynamic(this, &UMPInventoryUI::CloseInventory);
	}
	
}

void UMPInventoryUI::SetSlotsArray(UPanelWidget* InParentContainer)
{
	TArray<UWidget*> Widgets = InParentContainer->GetAllChildren();
	
	InventorySlots.Empty();
	for (UWidget* Widget : Widgets)
	{
		if (UMPInventorySlot* InventorySlot = Cast<UMPInventorySlot>(Widget))
		{
			InventorySlots.AddUnique(InventorySlot);
		}
	}
}

void UMPInventoryUI::OnAddItem(const FItemPickedEvent& Event)
{
	BP_AddItem(Event.Item);
}

UMPInventorySlot* UMPInventoryUI::GetInventorySlotByItem(const UMPItem* Item)
{
	for (const auto InventorySlot : InventorySlots)
	{
		if (InventorySlot->GetItem() == Item)
		{
			return InventorySlot;
		}
	}

	UE_LOG(LogTemp, Fatal, TEXT("Item not found in InventorySlots"));
	return nullptr;
}

void UMPInventoryUI::OnUpdateItem(const FItemUpdatedEvent& ItemUpdatedEvent)
{
	BP_RemoveItem(GetInventorySlotByItem(ItemUpdatedEvent.Item));
	
	if (UMPItem* Item = ItemUpdatedEvent.Item->RemainingItemAfterInteraction)
		BP_AddItem(Item);
}

void UMPInventoryUI::OnRemoveItem(const FItemDroppedEvent& Event)
{
	BP_RemoveItem(Event.InventorySlot);
}

void UMPInventoryUI::ToggleInventory(const FInventoryToggle& Event)
{
	Event.bOpen? SetVisibility(ESlateVisibility::Visible) : SetVisibility(ESlateVisibility::Collapsed);
}

void UMPInventoryUI::CloseInventory()
{
	UEventRouterSubsystem::BroadcastEvent(this, InventoryUIEventsTag, FInventoryToggle(false));
}

void UMPInventoryUI::NativeDestruct()
{
	UEventRouterSubsystem::UnsubscribeFromEvent(this, InventoryUIEventsTag, InventoryPickedUpItemHandle);
	UEventRouterSubsystem::UnsubscribeFromEvent(this, InventoryUIEventsTag, InventoryToggleHandle);
	
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.RemoveDynamic(this, &UMPInventoryUI::CloseInventory);
	}
	Super::NativeDestruct();
}