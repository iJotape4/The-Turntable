// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TTBackKeyWidget.h"

void UTTBackKeyWidget::OnBackKeyPressed() const
{
	OnBackKeyPressedDelegate.Broadcast();
}
