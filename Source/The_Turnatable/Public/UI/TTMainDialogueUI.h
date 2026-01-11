// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Core/EventPayloads/LevelProgressPayloads.h"
#include "TTMainDialogueUI.generated.h"

/**
 *
 */
UCLASS()
class THE_TURNATABLE_API UTTMainDialogueUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Config")
	FGameplayTag DialoguesTag = FGameplayTag::RequestGameplayTag("UI.Dialogues", false);

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	FText SetDialogueText(const FText& InDialogueText);
	
	void DisplayText(const FGenericDialogueEvent& DialogueEvent);
	void CloseDialogue();

	virtual void NativeDestruct() override;
	
private:
	FText DialogueText;
	FTimerHandle TimerHandle;

	FDelegateHandle DialogueEventHandle;
};
