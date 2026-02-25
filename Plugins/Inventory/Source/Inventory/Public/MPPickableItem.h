// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPInteractableObject.h"

#include "MPPickableItem.generated.h"

class UMPItem;

UCLASS()
class INVENTORY_API AMPPickableItem : public AMPInteractableObject
{
	GENERATED_BODY()
public:
	UStaticMeshComponent* StaticMeshComponent;
	USkeletalMeshComponent* SkeletalMeshComponent;
	// Sets default values for this actor's properties
	AMPPickableItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	UMPItem* Item;
	
	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(CallInEditor, Category="Data")
	void ApplyDataAsset();

	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UMPItem* LastSubscribedItem;
#endif
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};