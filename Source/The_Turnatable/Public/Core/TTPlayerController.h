// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/HorrorPlayerController.h"
#include "TTPlayerController.generated.h"

class UMPGenericDialog;
class UMPInventoryUI;
/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API ATTPlayerController : public AHorrorPlayerController
{
	GENERATED_BODY()

protected:

	FGameplayTag InventoryUIEventsTag = FGameplayTag::RequestGameplayTag("UI.Inventory");
	
	UPROPERTY(EditAnywhere, Category="UI | Inventory")
	TSubclassOf<UMPInventoryUI> InventoryUIClass;
	
	UPROPERTY()
	TObjectPtr<UMPInventoryUI> InventoryUI;
	
	UPROPERTY(EditAnywhere, Category="UI | Dialogues")
	TSubclassOf<UMPGenericDialog> DialogsUIClass;
	
	UPROPERTY()
	TObjectPtr<UMPGenericDialog> DialogsUI;

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	
	UFUNCTION()
	void ToggleInventory(const FInventoryToggle& Event);
	
private:
	FDelegateHandle InventoryToggleHandle;
};