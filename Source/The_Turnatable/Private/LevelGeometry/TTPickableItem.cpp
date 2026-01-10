// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTPickableItem.h"

#include "Core/EventRouterSubsystem.h"
#include "Core/EventPayloads/InventoryEventPayloads.h"
#include "Core/TTInteractionComponent.h"

// Sets default values
ATTPickableItem::ATTPickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool ATTPickableItem::Interact_Implementation(APawn* InstigatorPawn)
{
	//Ensures the player is inside the interaction area
	if (!Super::Interact_Implementation(InstigatorPawn)) return false;
	
	if (ensureAlwaysMsgf(Item, TEXT("Item Data asset is not set on: %s"), *GetName()))
	{
		UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", FItemPickedEvent{Item});
		
		Destroy();
		return true;
		
	}
	return false;
}