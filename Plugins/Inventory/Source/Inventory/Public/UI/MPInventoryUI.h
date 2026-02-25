	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MPInventoryEventPayloads.h"
#include "Blueprint/UserWidget.h"
#include "MPInventoryUI.generated.h"

class UMPBackKeyWidget;
class UMPInventoryComponent;
class UMPInventorySlot;
class UMPItem;

UCLASS()
class INVENTORY_API UMPInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag InventoryUIEventsTag = FGameplayTag::RequestGameplayTag("UI.Inventory", false);
	
	void OnUpdateItem(const FItemUpdatedEvent& ItemUpdatedEvent);
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetSlotsArray(UPanelWidget* InParentContainer);
	
	UFUNCTION()
	void OnAddItem(const FItemPickedEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", meta = (DisplayName = "Add Item"))
	void BP_AddItem(UMPItem* Item);

	UFUNCTION()
	UMPInventorySlot* GetInventorySlotByItem(const UMPItem* Item);
	
	UFUNCTION()
	void OnRemoveItem(const FItemDroppedEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", meta = (DisplayName = "Remove Item"))
	void BP_RemoveItem(UMPInventorySlot* InventorySlot);
	UFUNCTION()
	void ToggleInventory(const FInventoryToggle& Event);

	void NativeDestruct() override;
	
protected:
	UMPInventoryComponent* InventoryComponent;
	UMPBackKeyWidget* BackKeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<UMPInventorySlot*> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName BackKeyWidgetName = "BackKeyWidget";

	UFUNCTION()
	void CloseInventory();

private:
	FDelegateHandle InventoryToggleHandle;
	FDelegateHandle InventoryPickedUpItemHandle;
	FDelegateHandle InventoryItemDroppedHandle;
	FDelegateHandle InventoryItemUpdatedHandle;
};