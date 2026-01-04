// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInventorySlot.h"

#include "Core/Inventory/TTItem.h"
#include "LevelGeometry/TTInspectItem.h"

void UTTInventorySlot::Onclick()
{
	if (Item)
	{
		UE_LOG(LogTemp, Warning, TEXT ("Item: %s"), *Item->ItemName.ToString());
		ATTInspectItem* InspectItem = GetWorld()->SpawnActor<ATTInspectItem>(InspectItemClass);
		InspectItem->InspectItem(Item->ItemMesh,Item->ItemName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT ("No Item"));
	}
}