// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelGeometry/TTItemDropZone.h"

#include "Components/BoxComponent.h"


// Sets default values
ATTItemDropZone::ATTItemDropZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DropZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("DropZoneMesh");
	RootComponent = DropZoneMesh;

	DropZoneCollider = CreateDefaultSubobject<UBoxComponent>("DropZoneCollider");
	DropZoneCollider->SetupAttachment(RootComponent);
}

bool ATTItemDropZone::Interact_Implementation(APawn* InstigatorPawn)
{
	return Super::Interact_Implementation(InstigatorPawn);
	
}

bool ATTItemDropZone::ReceiveItem(UTTItem* Item)
{
	if (Item == nullptr || Item != RequiredItem) return false;

	AcceptItem();
	return true;
}

void ATTItemDropZone::DispatchEvent_Implementation(const FInstancedStruct& Payload)
{
	UEventRouterSubsystem::BroadcastEvent(this, EventTopic, Payload);
}