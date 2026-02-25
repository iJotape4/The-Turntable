// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "MPItem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDataChanged);

UCLASS()
class INVENTORY_API UMPItem : public UDataAsset
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
	UStreamableRenderAsset* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ChildItem", meta=(ToolTip="Automatically set depending on item Mesh type"))
	bool bIsInteractable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ChildItem", meta=(ToolTip="Set this when the item is interactable", EditCondition ="bIsInteractable"))
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ChildItem", meta=(ToolTip="Set this when the item is interactable", EditCondition ="bIsInteractable"))
	UMPItem* ChildItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ChildItem", meta=(ToolTip="Set this when the item is interactable", EditCondition ="bIsInteractable"))
	UMPItem* RemainingItemAfterInteraction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Data", Transient)
	FRotator ItemRotation = FRotator::ZeroRotator;

	FOnDataChanged OnChanged;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		bIsInteractable = Cast<USkeletalMesh>(ItemMesh) != nullptr;
		
		OnChanged.Broadcast();
	}
#endif
};