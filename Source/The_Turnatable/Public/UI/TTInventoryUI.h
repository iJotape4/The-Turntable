// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/EventPayloads/InventoryEventPayloads.h"
#include "TTInventoryUI.generated.h"

class UTTInventorySlot;
class UTTBackKeyWidget;
class UTTInventoryComponent;
class UTTItem;
class AHorrorCharacter;
/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API UTTInventoryUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetSlotsArray(UPanelWidget* InParentContainer);
	
	UFUNCTION()
	void OnAddItem(const FItemPickedEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", meta = (DisplayName = "Add Item"))
	void BP_AddItem(UTTItem* Item);

	UFUNCTION()
	UTTInventorySlot* GetInventorySlotByItem(UTTItem* Item);
	
	UFUNCTION()
	// void OnRemoveItem(UTTItem* Item);
	void OnRemoveItem(const FSlotSelectedEvent& Event);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", meta = (DisplayName = "Remove Item"))
	void BP_RemoveItem(UTTInventorySlot* InventorySlot);
	UFUNCTION()
	void ToggleInventory(const FInventoryToggle& Event);

	void NativeDestruct() override;
	
protected:
	UTTInventoryComponent* InventoryComponent;
	UTTBackKeyWidget* BackKeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<UTTInventorySlot*> InventorySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName BackKeyWidgetName = "BackKeyWidget";

	UFUNCTION()
	void CloseInventory();

private:
	FDelegateHandle InventoryToggleHandle;
	FDelegateHandle InventoryPickedUpItemHandle;
	FDelegateHandle InventorySlotSelectedHandle;
};
