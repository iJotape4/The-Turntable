// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTInspectWidget.generated.h"

/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API UTTInspectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FText Item_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FText Item_Description;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInspect(const FText& ItemName, const FText& ItemDescription = FText::GetEmpty());
};