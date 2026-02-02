#pragma once
#include "CoreMinimal.h"
#include "InventoryEventPayloads.generated.h"

enum class EInventoryMode : uint8;
class UTTInventorySlot;
class UTTItem;
class ATTItemDropZone;

USTRUCT()
struct FInventoryToggle
{
	GENERATED_BODY()
	bool bOpen = true;
	EInventoryMode NewMode;
};

USTRUCT()
struct FItemPickedEvent
{
	GENERATED_BODY()
	UTTItem* Item;
};

USTRUCT()
struct FSlotSelectedEvent
{
	GENERATED_BODY()
	UTTInventorySlot* InventorySlot;
};

USTRUCT()
struct FItemUpdatedEvent
{
	GENERATED_BODY()
	UTTItem* Item;
};

USTRUCT()
struct FMatchKeyItemEvent
{
	GENERATED_BODY()
	ATTItemDropZone* DropZone;
};

USTRUCT()
struct FItemDroppedEvent
{
	GENERATED_BODY()
	UTTInventorySlot* InventorySlot;
};