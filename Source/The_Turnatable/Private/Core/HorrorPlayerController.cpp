// Copyright Epic Games, Inc. All Rights Reserved.


#include "Core/HorrorPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Core/The_TurnatableCameraManager.h"
#include "Core/HorrorCharacter.h"
#include "UI/HorrorUI.h"
#include "The_Turnatable.h"
#include "Core/EventRouterSubsystem.h"
#include "Core/Inventory/TTInventoryComponent.h"
#include "UI/TTInventoryUI.h"
#include "Widgets/Input/SVirtualJoystick.h"

AHorrorPlayerController::AHorrorPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AThe_TurnatableCameraManager::StaticClass();
}

void AHorrorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogThe_Turnatable, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AHorrorPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// only spawn UI on local player controllers
	if (IsLocalPlayerController())
	{
		// set up the UI for the character
		if (AHorrorCharacter* HorrorCharacter = Cast<AHorrorCharacter>(aPawn))
		{
			// create the UI
			if (!HorrorUI)
			{
				HorrorUI = CreateWidget<UHorrorUI>(this, HorrorUIClass);
				HorrorUI->AddToViewport(0);
				HorrorUI->SetupCharacter(HorrorCharacter);
			}

			if (!InventoryUI)
			{
				InventoryUI = CreateWidget<UTTInventoryUI>(this, InventoryUIClass);
				InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
				InventoryUI->AddToViewport();
				UEventRouterSubsystem::UnsubscribeFromEvent(this, "UI.Inventory", InventoryToggleHandle);
				InventoryToggleHandle = UEventRouterSubsystem::SubscribeToEvent<FInventoryToggle>(this, "UI.Inventory", &AHorrorPlayerController::ToggleInventory);
			}
		}
	}
}

void AHorrorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}	
}

bool AHorrorPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AHorrorPlayerController::ToggleInventory(const FInventoryToggle& Event)
{
	const bool bOpen = Event.bOpen;
	bEnableClickEvents = bOpen;
	bShowMouseCursor = bOpen;

	if (bOpen)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			for (UInputMappingContext* CurrentContext : InspectMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
	else
	{
		SetupInputComponent();
	}
}