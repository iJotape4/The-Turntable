// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTInventoryUI.generated.h"

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
	void SetUpInventoryComponent(AHorrorCharacter* HorrorCharacter);

	UFUNCTION()
	void OnAddItem(UTTItem* Item);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory", meta = (DisplayName = "Add Item"))
	void BP_AddItem(UTTItem* Item);
	UFUNCTION()
	void ToggleInventory(bool bOpen);
	
protected:
	UTTInventoryComponent* InventoryComponent;
	UTTBackKeyWidget* BackKeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName BackKeyWidgetName = "BackKeyWidget";

	UFUNCTION()
	void CloseInventory();
};
