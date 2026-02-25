// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MPInteractableObject.h"
#include "StructUtils/InstancedStruct.h"

#include "MPKeyItemDropZone.generated.h"

class UBoxComponent;
class UMPItem;

UCLASS(Abstract)
class INVENTORY_API AMPKeyItemDropZone : public AMPInteractableObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* DropZoneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* DropZoneCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TArray<UMPItem*> RequiredItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	bool bAcceptsItems = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag UIDialogEventsTag = FGameplayTag::RequestGameplayTag("UI.Dialogs", false);
	
	// Sets default values for this actor's properties
	AMPKeyItemDropZone();

	virtual bool Interact_Implementation(APawn* InstigatorPawn) override;
	virtual bool IsItemRequired(UMPItem* Item);
	bool ReceiveItem(const UMPItem* Item);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag EventTopic;
	
	FInstancedStruct ev;
	
	virtual void AcceptItem(UMPItem* Item) PURE_VIRTUAL(ATTItemDropZone::AcceptItem, );
	virtual void FinishPuzzle() PURE_VIRTUAL(ATTItemDropZone::FinishPuzzle,);

	template <typename TPayloadStruct>
	void DispatchEvent(const TPayloadStruct& Payload)
	{
		ev.InitializeAs<TPayloadStruct>(Payload);
		DispatchEvent_Implementation(ev);
	}

private:
	void DispatchEvent_Implementation(const FInstancedStruct& Payload);;
};