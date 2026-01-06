// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTInspectWidget.h"

#include "UI/TTBackKeyWidget.h"

void UTTInspectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("Constructed Inspect Widget with BackKeyWidgetName: %s"), *BackKeyWidgetName.ToString());
	BackKeyWidget = Cast<UTTBackKeyWidget>(GetWidgetFromName(BackKeyWidgetName));
	if (BackKeyWidget)
	{
		BackKeyWidget->OnBackKeyPressedDelegate.AddDynamic(this, &UTTInspectWidget::CloseInspectWidget);
	}
}

void UTTInspectWidget::CloseInspectWidget()
{
	OnCloseByBackKeyDelegate.Broadcast();
}