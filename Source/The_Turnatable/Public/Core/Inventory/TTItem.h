// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTItem.generated.h"

/**
 * 
 */
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

	// The icon representing the item in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	UStaticMesh* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", Transient)
	FRotator ItemRotation = FRotator::ZeroRotator;
};
