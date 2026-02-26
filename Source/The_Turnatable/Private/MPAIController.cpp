// Fill out your copyright notice in the Description page of Project Settings.


#include "MPAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void AMPAIController::BeginPlay()
{
	Super::BeginPlay();
	
	RunBehaviorTree(BehaviorTree);
	
	if (APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		{
			
			BlackboardComp->SetValueAsVector(MoveToLocationKeyName, MyPawn->GetActorLocation());
			BlackboardComp->SetValueAsObject(TargetActorKeyName, MyPawn);
		}
	}
}