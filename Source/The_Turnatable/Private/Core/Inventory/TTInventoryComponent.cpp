// Fill out your copyright notice in the Description page of Project Settings.


#include "TTInventoryComponent.h"

#include "Core/HorrorCharacter.h"
#include "Core/TTInteractionComponent.h"
#include "Core/Inventory/TTItem.h"


// Sets default values for this component's properties
UTTInventoryComponent::UTTInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTTInventoryComponent::PostInitProperties()
{
	Super::PostInitProperties();
	if (AActor* Owner = GetOwner())
	{
		if (UTTInteractionComponent* InteractionComponent = Owner->FindComponentByClass<UTTInteractionComponent>())
		{
			InteractionComponent->OnInteractDelegate.AddDynamic(this, &UTTInventoryComponent::AddItem);
		}
	}
}

void UTTInventoryComponent::AddItem(UTTItem* NewItem)
{
	Inventory.Add(NewItem);
	UE_LOG(LogTemp, Warning, TEXT("Added a new item to inventory: %s"), *NewItem->ItemName.ToString());
}

void UTTInventoryComponent::RemoveItem(UTTItem* ItemToRemove)
{
	Inventory.Remove(ItemToRemove);
}

bool UTTInventoryComponent::HasItem(UTTItem* ItemToCheck)
{
	return Inventory.Contains(ItemToCheck);
}
