// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MPInventorySlot.generated.h"

class UMPInventoryComponent;
class UMPItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UMPInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Onclick();
	void NativeConstruct() override;
	UMPItem* GetItem() const;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Data")
	UMPItem* Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UMPInventoryComponent* InventoryComponent;
};