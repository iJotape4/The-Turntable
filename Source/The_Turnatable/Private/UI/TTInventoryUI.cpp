// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInventoryUI.h"

#include "Components/PanelWidget.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/Inventory/TTInventoryComponent.h"
#include "Core/Inventory/TTItem.h"
#include "UI/TTBackKeyWidget.h"
#include "UI/TTInventorySlot.h"

void UTTInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	const FName UIEventsTag = FName("UI.Inventory");
	InventoryPickedUpItemHandle = UEventRouterSubsystem::SubscribeToEvent<FItemPickedEvent>(this, UIEventsTag, &UTTInventoryUI::OnAddItem);
	InventoryToggleHandle = UEventRouterSubsystem::SubscribeToEvent<FInventoryToggle>(this, UIEventsTag, &UTTInventoryUI::ToggleInventory);
	InventoryItemDroppedHandle = UEventRouterSubsystem::SubscribeToEvent<FItemDroppedEvent>(this,UIEventsTag,&UTTInventoryUI::OnRemoveItem);
	InventoryItemUpdatedHandle = UEventRouterSubsystem::SubscribeToEvent<FItemUpdatedEvent>(this,UIEventsTag,&UTTInventoryUI::OnUpdateItem);

	
	UE_LOG(LogTemp, Warning, TEXT("Constructed Inventory UI with BackKeyWidgetName: %s"), *BackKeyWidgetName.ToString());
	BackKeyWidget = Cast<UTTBackKeyWidget>(GetWidgetFromName(BackKeyWidgetName));
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.AddDynamic(this, &UTTInventoryUI::CloseInventory);
	}
	
}

void UTTInventoryUI::SetSlotsArray(UPanelWidget* InParentContainer)
{
	TArray<UWidget*> Widgets = InParentContainer->GetAllChildren();
	
	InventorySlots.Empty();
	for (UWidget* Widget : Widgets)
	{
		if (UTTInventorySlot* InventorySlot = Cast<UTTInventorySlot>(Widget))
		{
			InventorySlots.AddUnique(InventorySlot);
		}
	}
}

void UTTInventoryUI::OnAddItem(const FItemPickedEvent& Event)
{
	BP_AddItem(Event.Item);
}

UTTInventorySlot* UTTInventoryUI::GetInventorySlotByItem(UTTItem* Item)
{
	for (auto InventorySlot : InventorySlots)
	{
		if (InventorySlot->GetItem() == Item)
		{
			return InventorySlot;
		}
	}

	UE_LOG(LogTemp, Fatal, TEXT("Item not found in InventorySlots"));
	return nullptr;
}

void UTTInventoryUI::OnUpdateItem(const FItemUpdatedEvent& ItemUpdatedEvent)
{
	BP_RemoveItem(GetInventorySlotByItem(ItemUpdatedEvent.Item));
	
	if (UTTItem* Item = ItemUpdatedEvent.Item->RemainingItemAfterInteraction)
		BP_AddItem(Item);
}

void UTTInventoryUI::OnRemoveItem(const FItemDroppedEvent& Event)
{
	BP_RemoveItem(Event.InventorySlot);
}

void UTTInventoryUI::ToggleInventory(const FInventoryToggle& Event)
{
	Event.bOpen? SetVisibility(ESlateVisibility::Visible) : SetVisibility(ESlateVisibility::Collapsed);
}

void UTTInventoryUI::CloseInventory()
{
	UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", FInventoryToggle(false));
}

void UTTInventoryUI::NativeDestruct()
{
	UEventRouterSubsystem::UnsubscribeFromEvent(this, "UI.Inventory", InventoryPickedUpItemHandle);
	UEventRouterSubsystem::UnsubscribeFromEvent(this, "UI.Inventory", InventoryToggleHandle);
	
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.RemoveDynamic(this, &UTTInventoryUI::CloseInventory);
	}
	Super::NativeDestruct();
}