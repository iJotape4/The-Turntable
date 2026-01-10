// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/EventPayloads/LevelProgressPayloads.h"
#include "LevelGeometry/TTItemDropZone.h"
#include "TTItemDropZoneChildExample.generated.h"

UCLASS()
class THE_TURNATABLE_API ATTItemDropZoneChildExample : public ATTItemDropZone
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATTItemDropZoneChildExample();
	void DummyTest(const FDoorUnlockedEvent& event);
	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void AcceptItem() override;
};