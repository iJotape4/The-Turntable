#pragma once
#include "CoreMinimal.h"
#include "InventoryEventPayloads.generated.h"

class UTTInventorySlot;
class UTTItem;

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

USTRUCT()
struct FItemDroppedEvent
{
	GENERATED_BODY()
	UTTItem* Item;
};

USTRUCT()
struct FInventoryToggle
{
	GENERATED_BODY()
	bool bOpen = true;
};

USTRUCT()
struct FSlotSelectedEvent
{
	GENERATED_BODY()
	UTTInventorySlot* InventorySlot;
};