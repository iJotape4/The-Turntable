// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTBackKeyWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackKeyPressed);
/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API UTTBackKeyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnBackKeyPressed OnBackKeyPressedDelegate;

	UFUNCTION(BlueprintCallable)
	void OnBackKeyPressed() const;
};
