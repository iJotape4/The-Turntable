// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTInventoryComponent.generated.h"


class UTTItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THE_TURNATABLE_API UTTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	TArray<UTTItem*> Inventory;
	// Sets default values for this component's properties
	
public:
	UTTInventoryComponent();

	virtual void PostInitProperties() override;

	UFUNCTION()
	void AddItem(UTTItem* NewItem);
	UFUNCTION()
	void RemoveItem(UTTItem* ItemToRemove);
	
	bool HasItem(UTTItem* ItemToCheck);
};
