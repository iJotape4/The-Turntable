// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTInventorySlot.generated.h"

class UTTInventoryComponent;
class ATTInspectItem;
class UTTItem;
/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API UTTInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Onclick();
	void NativeConstruct() override;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Data")
	UTTItem* Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTTInventoryComponent* InventoryComponent;

};
