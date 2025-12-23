// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTInteractableObject.h"
#include "Core/HorrorCharacter.h"
#include "Components/SphereComponent.h"

void ATTInteractableObject::Interact_Implementation(APawn* InstigatorPawn)
{
	ITTGameplayInterface::Interact_Implementation(InstigatorPawn);
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AHorrorCharacter::StaticClass());
	if (OverlappingActors.Num() <= 0) return;

	UE_LOG(LogTemp, Warning, TEXT("Interacting with %s"), *GetName());
}

// Sets default values
ATTInteractableObject::ATTInteractableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionArea");
	SphereComponent->SetSphereRadius(InteractionRadius);
}

// Called when the game starts or when spawned
void ATTInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATTInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

