#include "MPInteractionComponent.h"

#include "MPGameplayInterface.h"

void UMPInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + ( EyeRotation.Vector() * 100000.f );

	//Pixel perfect implementation
	//FHitResult Hit;
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> HitResults;

	float SpehereRadius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(SpehereRadius);

	bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugColor = bHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UMPGameplayInterface>())
			{
				IMPGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(MyOwner));
				DrawDebugSphere(GetWorld(), Hit.Location, SpehereRadius, 32, DebugColor, false, 2.0f);
				break;
			}
		}
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.0f, 2.0f);
}

// Sets default values for this component's properties
UMPInteractionComponent::UMPInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}