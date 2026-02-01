// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Core/EventPayloads/InventoryEventPayloads.h"
#include "TTInventoryComponent.generated.h"

class UTTInventorySlot;
struct FInputActionValue;
class ATTInspectItem;
class UTTItem;

UENUM(BlueprintType)
enum class EInventoryMode : uint8
{
	Default,
	MatchKeyItem,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THE_TURNATABLE_API UTTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class  ATTInspectItem> InspectItemClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	ATTInspectItem* InspectItemActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag UIEventsTag = FGameplayTag::RequestGameplayTag("UI.Inventory", false);
	FGameplayTag UIDialogueEventsTag = FGameplayTag::RequestGameplayTag("UI.Dialogues", false);
	
protected:
	TArray<UTTItem*> Inventory;
	// Sets default values for this component's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	bool bInventoryOpen =false;

public:
	UTTInventoryComponent();

	virtual void PostInitProperties() override;

	void MatchKeyItemEvent(const FMatchKeyItemEvent& MatchKeyItemEvent);
	virtual void BeginPlay() override;

	UFUNCTION()
	void AddItem(const FItemPickedEvent& Ev);
	UFUNCTION()
	void RemoveItem(UTTItem* ItemToRemove);
	void RemoveItem(const FItemDroppedEvent& Event);
	
	bool HasItem(TArray<UTTItem*> ItemsToCheck) const;
	void ToggleInventory();
	void ToggleInventory(const FInventoryToggle& Event);
	void SlotSelected(const FSlotSelectedEvent& SlotSelectedEvent);

	void RotateItem(const FInputActionValue& Value);
	void CloseInspectView();

	virtual void BeginDestroy() override;
	void OnPointerDown(const FInputActionValue& Value);
	void OnZoom(const FInputActionValue& InputActionValue);

private:
	FDelegateHandle InventoryPickedUpHandle;
	FDelegateHandle InventoryToggleHandle;
	FDelegateHandle InventoryMatchItemHandle;
	FDelegateHandle InventorySlotSelectedHandle;
	FDelegateHandle InventoryItemDroppedHandle;
	
	EInventoryMode InventoryMode = EInventoryMode::Default;

	ATTItemDropZone* CurrentDropZone = nullptr;
};