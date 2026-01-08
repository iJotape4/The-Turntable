// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Inventory/TTInventoryComponent.h"
#include "InputActionValue.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/EventPayloads/InventoryEventPayloads.h"
#include "Core/TTInteractionComponent.h"
#include "LevelGeometry/TTInspectItem.h"
#include "GameplayTagsManager.h"
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
	if (AActor* Owner = GetOwner())
	{
		if (UTTInteractionComponent* InteractionComponent = Owner->FindComponentByClass<UTTInteractionComponent>())
		{
			InteractionComponent->OnInteractDelegate.AddDynamic(this, &UTTInventoryComponent::AddItem);
		}
		this->OnSlotClickedDelegate.AddDynamic(this, &UTTInventoryComponent::RemoveItem);
	}
}

void UTTInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InspectItemActor = GetWorld()->SpawnActor<ATTInspectItem>(InspectItemClass);
	
	UEventRouterSubsystem* Router = GetWorld()->GetGameInstance()->GetSubsystem<UEventRouterSubsystem>();

	const FGameplayTag TopicUI = FGameplayTag::RequestGameplayTag(TEXT("UI"));
	const FGameplayTag TopicInventory = FGameplayTag::RequestGameplayTag(TEXT("UI.Inventory"));

	// Subscribe to only inventory events:
	InventoryHandle = Router->SubscribeTyped<FItemPickedEvent>(
		TopicInventory,
		this,
		&UTTInventoryComponent::OnInventoryChanged
	);
}

void UTTInventoryComponent::AddItem(UTTItem* NewItem)
{
	Inventory.Add(NewItem);
}

void UTTInventoryComponent::RemoveItem(UTTItem* ItemToRemove)
{
	Inventory.Remove(ItemToRemove);
}

void UTTInventoryComponent::RemoveItem(UTTInventorySlot* Slot)
{
	Inventory.Remove(Slot->GetItem());
}

bool UTTInventoryComponent::HasItem(UTTItem* ItemToCheck)
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
		OnInventoryToggleDelegate.Broadcast(bInventoryOpen);
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

void UTTInventoryComponent::OnInventoryChanged(const FItemPickedEvent& Ev)
{
	UE_LOG(LogTemp, Warning, TEXT ("Inventory Changed"));
	UE_LOG(LogTemp, Warning, TEXT ("Item: %s"), *Ev.Item->GetName());
}

void UTTInventoryComponent::BeginDestroy()
{
	if (UWorld* World = GetWorld())
	{
		if (UEventRouterSubsystem* Router = World->GetGameInstance()->GetSubsystem<UEventRouterSubsystem>())
		{
			const FGameplayTag TopicInventory = FGameplayTag::RequestGameplayTag(TEXT("UI.Inventory"));
			Router->Unsubscribe(TopicInventory, InventoryHandle);
		}
	}
	Super::BeginDestroy();
}