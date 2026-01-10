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
}

void ATTItemDropZone::ReceiveItem(UTTItem* Item)
{
	if (Item == nullptr || Item != RequiredItem) return;

	AcceptItem();
}

void ATTItemDropZone::AcceptItem()
{
	
}

void ATTItemDropZone::DummyTest(const FDoorUnlockedEvent& event)
{
	UE_LOG(LogTemp, Warning, TEXT("Dummy Test"));
}


// Called when the game starts or when spawned
void ATTItemDropZone::BeginPlay()
{
	Super::BeginPlay();
	UEventRouterSubsystem::SubscribeToEvent(this, "UI.Inventory", &ATTItemDropZone::DummyTest);
	DispatchEvent(ev);
}

void ATTItemDropZone::DispatchEvent_Implementation(const FInstancedStruct& Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Dispatching ItemDropZone Event"));
	UEventRouterSubsystem::BroadcastEvent(this, "UI.Inventory", Payload);
}

// Called every frame
void ATTItemDropZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

