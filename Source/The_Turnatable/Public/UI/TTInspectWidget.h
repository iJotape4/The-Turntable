// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTInspectWidget.generated.h"

class UTTBackKeyWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseByBackKey);
UCLASS()
class THE_TURNATABLE_API UTTInspectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FText Item_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FText Item_Description;

	FOnCloseByBackKey OnCloseByBackKeyDelegate;

	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInspect(const FText& ItemName, const FText& ItemDescription = FText::GetEmpty());
	
	UFUNCTION()
	void CloseInspectWidget();
	
protected:
	UTTBackKeyWidget* BackKeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName BackKeyWidgetName = "BackKeyWidget";
};