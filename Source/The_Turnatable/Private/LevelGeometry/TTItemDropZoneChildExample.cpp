// Fill out your copyright notice in the Description page of Project Settings.


#include "TTItemDropZoneChildExample.h"
#include "Core/EventPayloads/LevelProgressPayloads.h"


// Sets default values
ATTItemDropZoneChildExample::ATTItemDropZoneChildExample()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATTItemDropZoneChildExample::DummyTest(const FDoorUnlockedEvent& event)
{
	UE_LOG(LogTemp, Warning, TEXT("Dummy Test"));
}

// Called when the game starts or when spawned
void ATTItemDropZoneChildExample::BeginPlay()
{
	Super::BeginPlay();
	UEventRouterSubsystem::SubscribeToEvent(this, EventTopic.GetTagName(), &ATTItemDropZoneChildExample::DummyTest);
	DispatchEvent(FDoorUnlockedEvent());
}

// Called every frame
void ATTItemDropZoneChildExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}