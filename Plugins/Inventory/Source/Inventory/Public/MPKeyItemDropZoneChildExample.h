// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPayloads.h"
#include "MPKeyItemDropZone.h"
#include "MPKeyItemDropZoneChildExample.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API AMPKeyItemDropZoneChildExample : public AMPKeyItemDropZone
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMPKeyItemDropZoneChildExample();
	void DummyTest(const FDummyTestEvent& event);
	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void AcceptItem(UMPItem* Item) override;
	virtual void FinishPuzzle() override;
};