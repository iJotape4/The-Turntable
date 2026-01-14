// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeometry/TTInspectItem.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Core/Inventory/TTItem.h"
#include "Engine/TextureRenderTarget2D.h"
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
	//TODO delete return
	return;
	/*FRotator NewRotation = UKismetMathLibrary::ComposeRotators(StaticMeshComponent->GetComponentRotation(),
		FRotator(LookAxisVector.Y, LookAxisVector.X, 0.0f));
	StaticMeshComponent->SetWorldRotation(NewRotation);*/
}

void ATTInspectItem::CheckHitResult()
{
	UE_LOG(LogTemp, Warning, TEXT("OnPointerDown"));
	FHitResult HitResult;
	float U = InspectWidget->UV.X;
	float V = InspectWidget->UV.Y;
	UE_LOG(LogTemp, Warning, TEXT("UV: %f, %f"), U, V);
	FVector Start, End;
	if (RayFromSceneCaptureUV(SceneCaptureComponent2D, U, V, 100000.0f, Start, End))
	{
		FHitResult Hit;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(SceneCapturePick), true);
		// Optionally ignore some actors:
		// Params.AddIgnoredActor(SomeActor);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit, Start, End, ECC_Visibility, Params
		);

		if (bHit)
		{
			AActor* DSM= GetWorld()->SpawnActor<AActor>(DebugStaticMesh);
			DSM->SetActorLocation(Hit.ImpactPoint);
			DSM->SetActorRotation(FRotator::ZeroRotator);
			UE_LOG(LogTemp, Warning, TEXT("=================="));
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.GetActor()->GetActorLabel());
			UE_LOG(LogTemp, Warning, TEXT("ImpactPoint: %s"), *Hit.ImpactPoint.ToString());
			UE_LOG(LogTemp, Warning, TEXT("FaceIndex: %d"), Hit.FaceIndex);
			UE_LOG(LogTemp, Warning, TEXT("Component: %s"), *Hit.Component->GetName());
		}
	}
}

bool ATTInspectItem::RayFromSceneCaptureUV(USceneCaptureComponent2D* Capture, float U, float V, float TraceDistance,
	FVector& OutStart, FVector& OutEnd)
{
	if (!Capture) return false;

	// 1) UV -> NDC
	const float x = 2.0f * U - 1.1f;
	const float y = 0.6f - 2.0f * V;

	// 2) Get FOV and aspect
	const float FovDeg = Capture->FOVAngle;
	const float TanHalfFov = FMath::Tan(FMath::DegreesToRadians(FovDeg * 0.5f));

	float Aspect = 1.0f;
	if (Capture->TextureTarget)
	{
		const float W = (float)Capture->TextureTarget->SizeX;
		const float H = (float)Capture->TextureTarget->SizeY;
		if (H > 0.0f) Aspect = W / H;
	}

	// 3) Direction in capture camera space (UE camera looks along +X in view space? Actually for this math we treat forward as +Z.
	// We'll instead build in "camera forward = +X" convention to match UE component space.
	// In UE component space: X forward, Y right, Z up.
	FVector DirCam;
	DirCam.X = 1.0f;
	DirCam.Y = x * TanHalfFov * Aspect;
	DirCam.Z = y * TanHalfFov;
	DirCam.Normalize();

	const FVector Start = Capture->GetComponentLocation();
	const FRotator Rot = Capture->GetComponentRotation();
	const FVector DirWorld = Rot.RotateVector(DirCam);

	OutStart = Start;
	OutEnd   = Start + DirWorld * TraceDistance;
	return true;
}


void ATTInspectItem::CloseInspectWidget()
{
	if (!InspectWidget) return;
	LastInspectedItem->ItemRotation = StaticMeshComponent->GetComponentRotation();
	InspectWidget->OnCloseByBackKeyDelegate.RemoveDynamic(this, &ATTInspectItem::CloseInspectWidget);
	InspectWidget->RemoveFromParent();
	bIsInspecting = false;
}
