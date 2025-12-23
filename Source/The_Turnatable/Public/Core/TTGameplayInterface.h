// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TTGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTTGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THE_TURNATABLE_API ITTGameplayInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
