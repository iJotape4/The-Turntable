// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInventoryUI.h"

#include "Components/PanelWidget.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/HorrorCharacter.h"
#include "Core/Inventory/TTInventoryComponent.h"
#include "UI/TTBackKeyWidget.h"
#include "UI/TTInventorySlot.h"

void UTTInventoryUI::NativeConstruct()
{
	InventoryPickedUpItemHandle = UEventRouterSubsystem::SubscribeToEvent<FItemPickedEvent>(this, "UI.Inventory", &UTTInventoryUI::OnAddItem);
	InventoryToggleHandle = UEventRouterSubsystem::SubscribeToEvent<FInventoryToggle>(this, "UI.Inventory", &UTTInventoryUI::ToggleInventory);
	Super::NativeConstruct();
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

void UTTInventoryUI::SetUpInventoryComponent(AHorrorCharacter* HorrorCharacter)
{
	InventoryComponent = HorrorCharacter->GetComponentByClass<UTTInventoryComponent>();
	if (InventoryComponent)
	{
		InventoryComponent->OnSlotClickedDelegate.AddDynamic(this, &UTTInventoryUI::OnRemoveItem);
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

// void UTTInventoryUI::OnRemoveItem(UTTItem* Item)
// {
// 	BP_RemoveItem(GetInventorySlotByItem(Item));
// }

void UTTInventoryUI::OnRemoveItem(UTTInventorySlot* InventorySlot)
{
	BP_RemoveItem(InventorySlot);
}

void UTTInventoryUI::ToggleInventory(const FInventoryToggle& Event)
{
	Event.bOpen? SetVisibility(ESlateVisibility::Visible) : SetVisibility(ESlateVisibility::Collapsed);
}

void UTTInventoryUI::CloseInventory()
{
	if (!InventoryComponent) return;
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