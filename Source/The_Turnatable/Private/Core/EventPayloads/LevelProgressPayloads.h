// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelProgressPayloads.generated.h"

USTRUCT()
struct FGenericDialogueEvent
{
	GENERATED_BODY()
	FText Text;
	float Duration= 4.0f;
};

USTRUCT()
struct FDoorUnlockedEvent
{
	GENERATED_BODY()	
};