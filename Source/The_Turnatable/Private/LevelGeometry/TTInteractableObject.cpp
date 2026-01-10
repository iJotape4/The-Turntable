// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTInteractableObject.h"
#include "Core/HorrorCharacter.h"
#include "Components/SphereComponent.h"

bool ATTInteractableObject::Interact_Implementation(APawn* InstigatorPawn)
{
	ITTGameplayInterface::Interact_Implementation(InstigatorPawn);
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AHorrorCharacter::StaticClass());
	if (OverlappingActors.Num() <= 0) return false;
	
	UE_LOG(LogTemp, Warning, TEXT("Interacting with %s"), *GetName());
	return true;
}

// Sets default values
ATTInteractableObject::ATTInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionArea");
	SphereComponent->SetSphereRadius(InteractionRadius);
}