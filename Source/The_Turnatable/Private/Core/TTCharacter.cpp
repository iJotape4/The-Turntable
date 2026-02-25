// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TTCharacter.h"

#include "EnhancedInputComponent.h"
#include "MPInspectableInventoryComponent.h"
#include "MPInteractionComponent.h"

void ATTCharacter::TTCharacter()
{
	InteractionComponent = CreateDefaultSubobject<UMPInteractionComponent>(TEXT("Interaction Component"));
	InventoryComponent = CreateDefaultSubobject<UMPInspectableInventoryComponent>(TEXT("Inventory Component"));
}

void ATTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, InteractionComponent	, &UMPInteractionComponent::PrimaryInteract);

		//Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, InventoryComponent, &UMPInventoryComponent::ToggleInventory);

		//Inspection
		EnhancedInputComponent->BindAction(CloseInspectingViewAction, ETriggerEvent::Started, InventoryComponent, &UMPInspectableInventoryComponent::CloseInspectView);
		EnhancedInputComponent->BindAction(PointerDownAction, ETriggerEvent::Started, InventoryComponent, &UMPInspectableInventoryComponent::OnPointerDown);
		EnhancedInputComponent->BindAction(RotateInspectedItemAction, ETriggerEvent::Triggered, this, &ATTCharacter::HandleRotateItem);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATTCharacter::HandleZoom);
	}
}

void ATTCharacter::HandleRotateItem(const FInputActionValue& InputActionValue)
{
	if (!InventoryComponent) return;
	
	const FVector2D Axis = InputActionValue.Get<FVector2D>();
	InventoryComponent->RotateItem(Axis);
}

void ATTCharacter::HandleZoom(const FInputActionValue& InputActionValue)
{
	if (!InventoryComponent) return;
	
	const float Axis = InputActionValue.Get<float>();
	InventoryComponent->OnZoom(Axis);
}