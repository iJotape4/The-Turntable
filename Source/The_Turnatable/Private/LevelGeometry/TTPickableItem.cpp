// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTPickableItem.h"

#include "Core/TTInteractionComponent.h"


// Sets default values
ATTPickableItem::ATTPickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATTPickableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATTPickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATTPickableItem::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!Super::Interact_Implementation(InstigatorPawn)) return false;

	if (ensureAlwaysMsgf(Item, TEXT("Item Data asset is not set on: %s"), *GetName()))
	{
		if (UTTInteractionComponent* InteractionComponent = InstigatorPawn->GetComponentByClass<UTTInteractionComponent>())
		{
			InteractionComponent->OnInteractDelegate.Broadcast(Item);
			Destroy();
			return true;
		}
	}
	return false;
}