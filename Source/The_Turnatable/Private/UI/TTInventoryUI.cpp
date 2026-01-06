// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInventoryUI.h"

#include "Core/HorrorCharacter.h"
#include "Core/TTInteractionComponent.h"
#include "Core/Inventory/TTInventoryComponent.h"
#include "UI/TTBackKeyWidget.h"

void UTTInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();
	BackKeyWidget = Cast<UTTBackKeyWidget>(GetWidgetFromName(BackKeyWidgetName));
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.AddDynamic(this, &UTTInventoryUI::CloseInventory);
	}
}

void UTTInventoryUI::SetUpInventoryComponent(AHorrorCharacter* HorrorCharacter)
{
	if (UTTInteractionComponent* InteractionComponent = HorrorCharacter->GetComponentByClass<UTTInteractionComponent>())
	{
		InteractionComponent->OnInteractDelegate.AddDynamic(this, &UTTInventoryUI::OnAddItem);
	}

	InventoryComponent = HorrorCharacter->GetComponentByClass<UTTInventoryComponent>();
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryToggleDelegate.AddDynamic(this, &UTTInventoryUI::ToggleInventory);
	}
}

void UTTInventoryUI::OnAddItem(UTTItem* Item)
{
	BP_AddItem(Item);
}

void UTTInventoryUI::ToggleInventory(bool bOpen)
{
	bOpen? AddToViewport() : RemoveFromParent();
}


void UTTInventoryUI::CloseInventory()
{
	if (!InventoryComponent) return;
	InventoryComponent->OnInventoryToggleDelegate.Broadcast(false);
}
