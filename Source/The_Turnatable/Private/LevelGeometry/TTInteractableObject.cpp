// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTInteractableObject.h"
#include "Core/HorrorCharacter.h"
#include "Components/SphereComponent.h"

ATTInteractableObject::ATTInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionArea");
	SphereComponent->SetSphereRadius(InteractionRadius);
}

bool ATTInteractableObject::Interact_Implementation(APawn* InstigatorPawn)
{
	ITTGameplayInterface::Interact_Implementation(InstigatorPawn);
	return IsPlayerInRadius();
}

bool ATTInteractableObject::IsPlayerInRadius() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AHorrorCharacter::StaticClass());
	if (OverlappingActors.Num() <= 0) return false;
	
	UE_LOG(LogTemp, Warning, TEXT("Interacting with %s"), *GetName());
	return true;
}