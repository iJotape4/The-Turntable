// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Core/TTGameplayInterface.h"
#include "TTInteractableObject.generated.h"

class USphereComponent;

UCLASS(Abstract)
class THE_TURNATABLE_API ATTInteractableObject : public AActor , public ITTGameplayInterface
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category="Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float InteractionRadius = 150.0f;
	
	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;

	// Sets default values for this actor's properties
	ATTInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
