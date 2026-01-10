// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TTInteractableObject.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/EventPayloads/LevelProgressPayloads.h"
#include "TTItemDropZone.generated.h"

class UBoxComponent;
class UTTItem;



UCLASS()
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
	void DummyTest(const FDoorUnlockedEvent& event);

	FDoorUnlockedEvent ev;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	UTTItem*RequiredItem;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	template <typename TPayloadStruct>
	void DispatchEvent(const TPayloadStruct& Payload)
	{
		FInstancedStruct Boxed;
		Boxed.InitializeAs<TPayloadStruct>(Payload);
		DispatchEvent_Implementation(Boxed);
	}

	virtual void DispatchEvent_Implementation(const FInstancedStruct& Payload);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};