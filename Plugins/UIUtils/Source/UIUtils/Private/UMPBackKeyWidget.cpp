// Fill out your copyright notice in the Description page of Project Settings.


#include "UMPBackKeyWidget.h"

void UMPBackKeyWidget::OnBackKeyPressed() const
{
	OnBackKeyPressedDelegate.Broadcast();
}