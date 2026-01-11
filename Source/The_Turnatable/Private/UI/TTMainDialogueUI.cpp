// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/TTMainDialogueUI.h"

#include "Core/EventRouterSubsystem.h"

void UTTMainDialogueUI::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
	DialogueEventHandle = UEventRouterSubsystem::SubscribeToEvent<FGenericDialogueEvent>(this, DialoguesTag, &UTTMainDialogueUI::DisplayText);
}

void UTTMainDialogueUI::DisplayText(const FGenericDialogueEvent& DialogueEvent)
{
	SetDialogueText(DialogueEvent.Text);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(TimerHandle);
		TimerManager.SetTimer(TimerHandle, this, &UTTMainDialogueUI::CloseDialogue, DialogueEvent.Duration, false);
	}
}

void UTTMainDialogueUI::CloseDialogue()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UTTMainDialogueUI::NativeDestruct()
{
	Super::NativeDestruct();
	UEventRouterSubsystem::UnsubscribeFromEvent(this, DialoguesTag, DialogueEventHandle);
}