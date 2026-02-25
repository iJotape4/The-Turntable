// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericPayloads.h"
#include "Blueprint/UserWidget.h"
#include "UMPGenericDialog.generated.h"

/**
 * 
 */
UCLASS()
class UIUTILS_API UMPGenericDialog : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag DialogsTag = FGameplayTag::RequestGameplayTag("UI.Dialogs", false);

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	FText SetDialogText(const FText& InDialogueText);
	
	void DisplayText(const FGenericUIDialogueEvent& DialogueEvent);
	void CloseDialog();

	virtual void NativeDestruct() override;
	
private:
	FText DialogText;
	FTimerHandle TimerHandle;

	FDelegateHandle DialogEventHandle;
};