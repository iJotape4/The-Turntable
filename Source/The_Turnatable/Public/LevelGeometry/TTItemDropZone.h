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

	UStaticMeshComponent* DropZoneMesh;
	UBoxComponent* DropZoneCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bAcceptsItems = true;
	
	// Sets default values for this actor's properties
	ATTItemDropZone();

	void ReceiveItem(UTTItem* Item);
	void AcceptItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	UTTItem*RequiredItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag EventTopic;
	
	FInstancedStruct ev;

	template <typename TPayloadStruct>
	void DispatchEvent(const TPayloadStruct& Payload)
	{
		ev.InitializeAs<TPayloadStruct>(Payload);
		DispatchEvent_Implementation(ev);
	}

	virtual void DispatchEvent_Implementation(const FInstancedStruct& Payload);;
};