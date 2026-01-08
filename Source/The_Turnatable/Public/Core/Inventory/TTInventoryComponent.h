// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTInventoryComponent.generated.h"

class UTTInventorySlot;
struct FInputActionValue;
class ATTInspectItem;
class UTTItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryToggle, bool, Open);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UTTInventorySlot*, InventorySlot);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THE_TURNATABLE_API UTTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class  ATTInspectItem> InspectItemClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	ATTInspectItem* InspectItemActor;
	
protected:
	TArray<UTTItem*> Inventory;
	// Sets default values for this component's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	bool bInventoryOpen =false;

public:
	UTTInventoryComponent();
	FOnInventoryToggle OnInventoryToggleDelegate;
	FOnSlotClicked OnSlotClickedDelegate;

	virtual void PostInitProperties() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void AddItem(UTTItem* NewItem);
	UFUNCTION()
	void RemoveItem(UTTItem* ItemToRemove);
	void RemoveItem(UTTInventorySlot* Slot);
	
	bool HasItem(UTTItem* ItemToCheck);
	void ToggleInventory();

	void RotateItem(const FInputActionValue& Value);
	void CloseInspectView();
};
