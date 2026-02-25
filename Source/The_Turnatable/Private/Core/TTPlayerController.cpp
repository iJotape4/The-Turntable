// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TTPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EventRouterSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UI/MPInventoryUI.h"
#include "UMPGenericDialog.h"

void ATTPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if ensure(!InventoryUI && InventoryUIClass)
	{
		InventoryUI = CreateWidget<UMPInventoryUI>(this, InventoryUIClass);
		InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
		InventoryUI->AddToViewport();
		//UEventRouterSubsystem::UnsubscribeFromEvent(this, InventoryUIEventsTag, InventoryToggleHandle);
		InventoryToggleHandle = UEventRouterSubsystem::SubscribeToEvent<FInventoryToggle>(this, InventoryUIEventsTag, &ATTPlayerController::ToggleInventory);
	}
	
	if ensure(!DialogsUI && DialogsUIClass)
	{
		DialogsUI = CreateWidget<UMPGenericDialog>(this, DialogsUIClass);
		DialogsUI->AddToViewport();
	}
}

void ATTPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	if (InventoryUI)
	{
		InventoryUI->RemoveFromParent();
		InventoryUI = nullptr;
		UEventRouterSubsystem::UnsubscribeFromEvent(this, InventoryUIEventsTag, InventoryToggleHandle);
	}
	if (DialogsUI)
	{
		DialogsUI->RemoveFromParent();
		DialogsUI = nullptr;
	}
}

void ATTPlayerController::ToggleInventory(const FInventoryToggle& Event)
{
	const bool bOpen = Event.bOpen;
	bEnableClickEvents = bOpen;
	bShowMouseCursor = bOpen;

	if (bOpen)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			SetInputMode(FInputModeGameAndUI());
			Subsystem->ClearAllMappings();
			for (UInputMappingContext* CurrentContext : InspectMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		SetupInputComponent();
	}
}