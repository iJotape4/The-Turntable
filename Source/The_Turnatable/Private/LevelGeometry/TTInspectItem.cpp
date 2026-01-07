// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTInspectItem.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Core/Inventory/TTItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/TTInspectWidget.h"

// Sets default values
ATTInspectItem::ATTInspectItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default SceneComponent"));
	RootComponent = SceneComponent;
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(SceneComponent);
	SceneCaptureComponent2D->SetRelativeLocation(FVector(-100.0f, 0.0f, 0.0f));
	SceneCaptureComponent2D->FOVAngle = 50.0f;
	SceneCaptureComponent2D->ShowFlags.Atmosphere = false;
	SceneCaptureComponent2D->bConsiderUnrenderedOpaquePixelAsFullyTranslucent = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(RootComponent);
	PointLightComponent->SetRelativeLocation(FVector(-100.0f, 0.0f, 0.0f));
}

void ATTInspectItem::InspectItem(UTTItem* Item)
{
	if (!InspectWidgetClass) return;
	
	StaticMeshComponent->SetStaticMesh(Item->ItemMesh);
	if (!InspectWidget)
	{
		InspectWidget = Cast<UTTInspectWidget>(CreateWidget<UUserWidget>(GetWorld(), InspectWidgetClass));
	}
	LastInspectedItem = Item;
	InspectWidget->OnInspect(Item->ItemName, Item->ItemDescription);
	InspectWidget->AddToViewport();
	bIsInspecting = true;
	StaticMeshComponent->SetWorldRotation(Item->ItemRotation);
	UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *Item->ItemRotation.ToString());
	InspectWidget->OnCloseByBackKeyDelegate.AddDynamic(this, &ATTInspectItem::CloseInspectWidget);
}


void ATTInspectItem::RotateItem(const FVector2D LookAxisVector) const
{
	FRotator NewRotation = UKismetMathLibrary::ComposeRotators(StaticMeshComponent->GetComponentRotation(),
		FRotator(LookAxisVector.Y, LookAxisVector.X, 0.0f));
	StaticMeshComponent->SetWorldRotation(NewRotation);
}

void ATTInspectItem::CloseInspectWidget()
{
	if (!InspectWidget) return;
	LastInspectedItem->ItemRotation = StaticMeshComponent->GetComponentRotation();
	InspectWidget->OnCloseByBackKeyDelegate.RemoveDynamic(this, &ATTInspectItem::CloseInspectWidget);
	InspectWidget->RemoveFromParent();
	bIsInspecting = false;
}
