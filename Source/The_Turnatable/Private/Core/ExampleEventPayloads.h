#pragma once
#include "CoreMinimal.h"
#include "Core/Inventory/TTItem.h"
#include "ExampleEventPayloads.generated.h"

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
struct FQuestUpdatedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FName QuestId;

	UPROPERTY()
	int32 NewStage = 0;
};
