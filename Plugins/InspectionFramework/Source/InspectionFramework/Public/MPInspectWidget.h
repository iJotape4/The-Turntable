// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MPInspectWidget.generated.h"

class UMPBackKeyWidget;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseByBackKey);

UCLASS()
class INSPECTIONFRAMEWORK_API UMPInspectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FText Item_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FText Item_Description;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Config")
	FVector2D UV;
	
	FOnCloseByBackKey OnCloseByBackKeyDelegate;

	UFUNCTION()
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInspect(const FText& ItemName, const FText& ItemDescription = FText::GetEmpty());
	
	UFUNCTION()
	void CloseInspectWidget();
	
protected:
	UMPBackKeyWidget* BackKeyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	FName BackKeyWidgetName = "BackKeyWidget";
};