// Fill out your copyright notice in the Description page of Project Settings.
#include "MPInspectWidget.h"
#include "UMPBackKeyWidget.h"

void UMPInspectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("Constructed Inspect Widget with BackKeyWidgetName: %s"), *BackKeyWidgetName.ToString());
	BackKeyWidget = Cast<UMPBackKeyWidget>(GetWidgetFromName(BackKeyWidgetName));
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.AddDynamic(this, &UMPInspectWidget::CloseInspectWidget);
	}
}

void UMPInspectWidget::CloseInspectWidget()
{
	OnCloseByBackKeyDelegate.Broadcast();
}