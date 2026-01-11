// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTItem.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDataChanged);

UCLASS()
class THE_TURNATABLE_API UTTItem : public UDataAsset
{
	GENERATED_BODY()	

public:
	// The display name of the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText ItemName;

	// The description of the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta=(MultiLine=true))
	FText ItemDescription;

	// The description of the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta=(MultiLine=true))
	FText PickupSentence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta=(MultiLine=true))
	FText DropOffSentence;
	
	// The icon representing the item in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	UStaticMesh* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", Transient)
	FRotator ItemRotation = FRotator::ZeroRotator;

	FOnDataChanged OnChanged;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		OnChanged.Broadcast();
	}
#endif
};