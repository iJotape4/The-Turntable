// Fill out your copyright notice in the Description page of Project Settings.


#include "UMPGenericDialog.h"

#include "EventRouterSubsystem.h"
#include "GenericPayloads.h"

#include "Components/SlateWrapperTypes.h"

void UMPGenericDialog::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
	DialogEventHandle = UEventRouterSubsystem::SubscribeToEvent<FGenericUIDialogueEvent>(this, DialogsTag, &UMPGenericDialog::DisplayText);
}

void UMPGenericDialog::DisplayText(const FGenericUIDialogueEvent& DialogueEvent)
{
	SetDialogText(DialogueEvent.Text);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(TimerHandle);
		TimerManager.SetTimer(TimerHandle, this, &UMPGenericDialog::CloseDialog, DialogueEvent.Duration, false);
	}
}

void UMPGenericDialog::CloseDialog()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UMPGenericDialog::NativeDestruct()
{
	Super::NativeDestruct();
	UEventRouterSubsystem::UnsubscribeFromEvent(this, DialogsTag, DialogEventHandle);
}