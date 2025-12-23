// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelGeometry/TTInteractableObject.h"
#include "TTPickableItem.generated.h"

class UTTItem;

UCLASS()
class THE_TURNATABLE_API ATTPickableItem : public ATTInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATTPickableItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	UTTItem* Item;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;
};
