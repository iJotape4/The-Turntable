// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MPAIController.generated.h"

/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API AMPAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category="Config")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category="Config|Keys")
	FName MoveToLocationKeyName = "MoveToLocation";

	UPROPERTY(EditDefaultsOnly, Category="Config|Keys")
	FName TargetActorKeyName = "TargetActor";
	
	
	virtual void BeginPlay() override;
};