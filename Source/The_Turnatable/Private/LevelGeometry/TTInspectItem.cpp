// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTInspectItem.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
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

// Called when the game starts or when spawned
void ATTInspectItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATTInspectItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTInspectItem::InspectItem(UStaticMesh* Mesh, const FText& ItemName, const FText& ItemDescription)
{
	if (!InspectWidgetClass) return;
	
	StaticMeshComponent->SetStaticMesh(Mesh);
	
	UTTInspectWidget* InspectWidget = Cast<UTTInspectWidget>(CreateWidget<UUserWidget>(GetWorld(), InspectWidgetClass));
	InspectWidget->OnInspect(ItemName, ItemDescription);
	InspectWidget->AddToViewport();
}