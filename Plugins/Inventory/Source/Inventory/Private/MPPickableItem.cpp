// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
#include "MPPickableItem.h"

#include "EventRouterSubsystem.h"
#include "MPInventoryEventPayloads.h"
#include "MPItem.h"
#include "Components/SphereComponent.h"

AMPPickableItem::AMPPickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	RootComponent = StaticMeshComponent;
	
	SphereComponent->SetupAttachment(StaticMeshComponent);
	SkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
}

bool AMPPickableItem::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!Super::Interact_Implementation(InstigatorPawn)) return false;
	
	if (ensureAlwaysMsgf(Item, TEXT("Item Data asset is not set on: %s"), *GetName()))
	{
		UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", FItemPickedEvent{Item});
		Destroy();
		return true;
		
	}
	return false;
}

void AMPPickableItem::ApplyDataAsset()
{
	if (!StaticMeshComponent || !Item) return;

	//UStaticMesh* NewMesh = (Item) ? Item->ItemMesh : nullptr; // if soft ptr
	// If MeshData->Mesh is a hard pointer, just: UStaticMesh* NewMesh = MeshData ? MeshData->Mesh : nullptr;

	UStreamableRenderAsset* ItemMesh = Item ? Item->ItemMesh : nullptr;
	if (!ItemMesh) return;

	if (Cast<UStaticMesh>(ItemMesh))
	{
		UE_LOG(LogTemp, Log, TEXT("UStaticMesh::InspectItem"));
		StaticMeshComponent->SetStaticMesh(Cast<UStaticMesh>(ItemMesh));
	}
	else if (Cast<USkeletalMesh>(ItemMesh))
	{
		UE_LOG(LogTemp, Log, TEXT("USkeletalMesh::InspectItem"));
		SkeletalMeshComponent->SetSkeletalMesh(Cast<USkeletalMesh>(ItemMesh));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unsupported mesh type for item: %s"), *Item->ItemName.ToString());
		return;
	}
}

void AMPPickableItem::OnConstruction(const FTransform& Transform)
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
			Item->OnChanged.AddUObject(this, &AMPPickableItem::ApplyDataAsset);
		}
	}
#endif

	ApplyDataAsset();
}


#if WITH_EDITOR
void AMPPickableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	if (PropName == GET_MEMBER_NAME_CHECKED(AMPPickableItem, Item))
	{
		OnConstruction(GetActorTransform());
	}
}
#endif
