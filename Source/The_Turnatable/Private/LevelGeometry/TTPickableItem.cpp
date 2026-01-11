// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelGeometry/TTPickableItem.h"

#include "Components/SphereComponent.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/EventPayloads/InventoryEventPayloads.h"
#include "Core/EventPayloads/LevelProgressPayloads.h"
#include "Core/Inventory/TTItem.h"

// Sets default values
ATTPickableItem::ATTPickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	SphereComponent->SetupAttachment(StaticMeshComponent);
}

bool ATTPickableItem::Interact_Implementation(APawn* InstigatorPawn)
{
	//Ensures the player is inside the interaction area
	if (!Super::Interact_Implementation(InstigatorPawn)) return false;
	
	if (ensureAlwaysMsgf(Item, TEXT("Item Data asset is not set on: %s"), *GetName()))
	{
		UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", FItemPickedEvent{Item});
		UEventRouterSubsystem::BroadcastEvent(this, "UI.Dialogues", FGenericDialogueEvent{Item->PickupSentence});
		Destroy();
		return true;
		
	}
	return false;
}

void ATTPickableItem::ApplyDataAsset()
{
	if (!StaticMeshComponent || !Item) return;

	//UStaticMesh* NewMesh = (Item) ? Item->ItemMesh : nullptr; // if soft ptr
	// If MeshData->Mesh is a hard pointer, just: UStaticMesh* NewMesh = MeshData ? MeshData->Mesh : nullptr;

	StaticMeshComponent->SetStaticMesh(Item->ItemMesh);
}

void ATTPickableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (LastSubscribedItem != Item)
	{
		if (LastSubscribedItem)
		{
			LastSubscribedItem->OnChanged.RemoveAll(this);
		}

		LastSubscribedItem = Item;

		if (Item)
		{
			Item->OnChanged.AddUObject(this, &ATTPickableItem::ApplyDataAsset);
		}
	}
#endif

	ApplyDataAsset();
}


#if WITH_EDITOR
void ATTPickableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (PropName == GET_MEMBER_NAME_CHECKED(ATTPickableItem, Item))
	{
		OnConstruction(GetActorTransform());
	}
}
#endif