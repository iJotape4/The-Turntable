// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelGeometry/TTItemDropZone.h"

#include "Components/SphereComponent.h"

// Sets default values
ATTItemDropZone::ATTItemDropZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DropZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("DropZoneMesh");
	RootComponent = DropZoneMesh;

	SphereComponent->SetupAttachment(DropZoneMesh);
}

bool ATTItemDropZone::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bAcceptsItems) return false;
	return Super::Interact_Implementation(InstigatorPawn);
}

bool ATTItemDropZone::ReceiveItem(UTTItem* Item)
{
	if (Item == nullptr || Item != RequiredItem) return false;

	bAcceptsItems = false;
	AcceptItem();
	return true;
}

void ATTItemDropZone::DispatchEvent_Implementation(const FInstancedStruct& Payload)
{
	UEventRouterSubsystem::BroadcastEvent(this, EventTopic, Payload);
}