#pragma once
#include "CoreMinimal.h"
#include "MPInventoryEventPayloads.generated.h"

enum class EInventoryMode : uint8;
class UMPInventorySlot;
class UMPItem;
class AMPKeyItemDropZone;

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
	UMPItem* Item;
};

USTRUCT()
struct FSlotSelectedEvent
{
	GENERATED_BODY()
	UMPInventorySlot* InventorySlot;
};

USTRUCT()
struct FItemUpdatedEvent
{
	GENERATED_BODY()
	UMPItem* Item;
};

USTRUCT()
struct FMatchKeyItemEvent
{
	GENERATED_BODY()
	AMPKeyItemDropZone* DropZone;
};

USTRUCT()
struct FItemDroppedEvent
{
	GENERATED_BODY()
	UMPInventorySlot* InventorySlot;
};