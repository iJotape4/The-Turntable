// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTInventorySlot.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class  ATTInspectItem> InspectItemClass;
	
	UFUNCTION(BlueprintCallable)
	void Onclick();
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Data")
	UTTItem* Item;
};
