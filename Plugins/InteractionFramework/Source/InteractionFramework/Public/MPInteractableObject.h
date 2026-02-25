#pragma once

#include "CoreMinimal.h"
#include "MPGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "MPInteractableObject.generated.h"

class USphereComponent;

UCLASS(Abstract)
class INTERACTIONFRAMEWORK_API AMPInteractableObject : public AActor, public IMPGameplayInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category="Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float InteractionRadius = 150.0f;
	
	AMPInteractableObject();
	
	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	bool IsPlayerInRadius() const;
};