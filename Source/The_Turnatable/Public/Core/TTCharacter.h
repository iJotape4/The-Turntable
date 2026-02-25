// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/HorrorCharacter.h"
#include "TTCharacter.generated.h"

class UMPInspectableInventoryComponent;
class UMPInteractionComponent;
/**
 * 
 */
UCLASS()
class THE_TURNATABLE_API ATTCharacter : public AHorrorCharacter
{
	GENERATED_BODY()
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UMPInteractionComponent* InteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UMPInspectableInventoryComponent* InventoryComponent;
	
protected:
	/** Interact input action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* InteractAction;

	/** Inventory input action */
	UPROPERTY(EditAnywhere, Category ="Input| Inventory")
	UInputAction* InventoryAction;

	/** Inventory input action */
	UPROPERTY(EditAnywhere, Category ="Input| Inventory")
	UInputAction* RotateInspectedItemAction;

	/** Inventory input action */
	UPROPERTY(EditAnywhere, Category ="Input| Inventory")
	UInputAction* CloseInspectingViewAction;
	
	UPROPERTY(EditAnywhere, Category ="Input| Inventory")
	UInputAction* PointerDownAction;

	UPROPERTY(EditAnywhere, Category ="Input| Inventory")
	UInputAction*	ZoomAction;
	ATTCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void HandleRotateItem(const FInputActionValue& InputActionValue);
	void HandleZoom(const FInputActionValue& InputActionValue);
};