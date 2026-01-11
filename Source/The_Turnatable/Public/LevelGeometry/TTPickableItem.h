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
	UStaticMeshComponent* StaticMeshComponent;
	// Sets default values for this actor's properties
	ATTPickableItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	UTTItem* Item;
	
	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(CallInEditor, Category="Data")
	void ApplyDataAsset();

	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UTTItem* LastSubscribedItem;
#endif
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};