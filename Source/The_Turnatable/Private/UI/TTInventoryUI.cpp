// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInventoryUI.h"

#include "Core/HorrorCharacter.h"
#include "Core/TTInteractionComponent.h"

void UTTInventoryUI::SetUpInventoryComponent(AHorrorCharacter* HorrorCharacter)
{
	if (UTTInteractionComponent* InteractionComponent = HorrorCharacter->GetComponentByClass<UTTInteractionComponent>())
	{
		InteractionComponent->OnInteractDelegate.AddDynamic(this, &UTTInventoryUI::OnAddItem);
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