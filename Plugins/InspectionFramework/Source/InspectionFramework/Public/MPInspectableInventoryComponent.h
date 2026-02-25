// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPInventoryComponent.h"
#include "MPInspectableInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class INSPECTIONFRAMEWORK_API UMPInspectableInventoryComponent : public UMPInventoryComponent
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class  AMPInspectItem> InspectItemClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	AMPInspectItem* InspectItemActor;

	virtual void BeginPlay() override;
	virtual void ToggleInventory() override;
	virtual void SlotSelected(const FSlotSelectedEvent& SlotSelectedEvent) override;

	void RotateItem(const FVector2D AxisValue) const;
	void CloseInspectView();

	void OnPointerDown();
	void OnZoom(const float Value) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config|Inspect")
	float RotationSpeed = 3.0f;
};