#include "MPInteractableObject.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

AMPInteractableObject::AMPInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionArea");
	SphereComponent->SetSphereRadius(InteractionRadius);
}

bool AMPInteractableObject::Interact_Implementation(APawn* InstigatorPawn)
{
	IMPGameplayInterface::Interact_Implementation(InstigatorPawn);
	return IsPlayerInRadius();
}

bool AMPInteractableObject::IsPlayerInRadius() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());
	if (OverlappingActors.Num() <= 0) return false;
	
	UE_LOG(LogTemp, Warning, TEXT("Interacting with %s"), *GetName());
	return true;
}