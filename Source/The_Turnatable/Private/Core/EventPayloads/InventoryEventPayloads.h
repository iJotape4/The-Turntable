#pragma once
#include "CoreMinimal.h"
#include "Core/Inventory/TTItem.h"
#include "InventoryEventPayloads.generated.h"

USTRUCT()
struct FInventoryChangedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY()
	UTTItem* Item = nullptr;
};

USTRUCT()
struct FItemPickedEvent
{
	GENERATED_BODY()
	UTTItem* Item;
};