// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TTInteractableObject.h"
#include "Core/EventRouterSubsystem.h"
#include "TTItemDropZone.generated.h"

class UBoxComponent;
class UTTItem;

UCLASS(Abstract)
class THE_TURNATABLE_API ATTItemDropZone : public ATTInteractableObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* DropZoneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* DropZoneCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	UTTItem*RequiredItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bAcceptsItems = true;
	
	// Sets default values for this actor's properties
	ATTItemDropZone();

	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;
	bool ReceiveItem(UTTItem* Item);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag EventTopic;
	
	FInstancedStruct ev;
	
	virtual void AcceptItem() PURE_VIRTUAL(ATTItemDropZone::AcceptItem, );

	template <typename TPayloadStruct>
	void DispatchEvent(const TPayloadStruct& Payload)
	{
		ev.InitializeAs<TPayloadStruct>(Payload);
		DispatchEvent_Implementation(ev);
	}

private:
	void DispatchEvent_Implementation(const FInstancedStruct& Payload);;
};