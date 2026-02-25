#include "MPInspectItem.h"

#include "EventRouterSubsystem.h"
#include "MPInspectWidget.h"
#include "MPInventoryEventPayloads.h"
#include "MPItem.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMPInspectItem::AMPInspectItem()
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
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	StaticMeshComponent->SetCollisionProfileName(FName("BlockAllDynamic"));

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(SceneComponent);
	SkeletalMeshComponent->SetCollisionProfileName(FName("BlockAllDynamic"));

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(RootComponent);
	PointLightComponent->SetRelativeLocation(FVector(-100.0f, 0.0f, 0.0f));
}

void AMPInspectItem::InspectItem(UMPItem* Item)
{
	if (!InspectWidgetClass || !Item) return;
	
	UStreamableRenderAsset* ItemMesh = Item->ItemMesh;
	
	if (Item->bIsInteractable)
	{
		UE_LOG(LogTemp, Log, TEXT("USkeletalMesh::InspectItem"));
		StaticMeshComponent->SetStaticMesh(nullptr);
		bIsInteractableItem = true;
		
		SkeletalMeshComponent->SetWorldRotation(Item->ItemRotation);
		SkeletalMeshComponent->SetSkeletalMesh(Cast<USkeletalMesh>(ItemMesh));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UStaticMesh::InspectItem"));
		SkeletalMeshComponent->SetSkeletalMesh(nullptr);
		bIsInteractableItem = false;
		
		StaticMeshComponent->SetWorldRotation(Item->ItemRotation);
		StaticMeshComponent->SetStaticMesh(Cast<UStaticMesh>(ItemMesh));
	}
	
	if (!InspectWidget)
	{
		InspectWidget = Cast<UMPInspectWidget>(CreateWidget<UUserWidget>(GetWorld(), InspectWidgetClass));
	}
	
	LastInspectedItem = Item;
	InspectWidget->OnInspect(Item->ItemName, Item->ItemDescription);
	InspectWidget->AddToViewport();
	bIsInspecting = true;
	
	UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *Item->ItemRotation.ToString());
	InspectWidget->OnCloseByBackKeyDelegate.AddDynamic(this, &AMPInspectItem::CloseInspectWidget);
}

void AMPInspectItem::RotateItem(const FVector2D LookAxisVector) const
{
	USceneComponent* MeshComponent = GetCurrentMeshComponent();
	
	FRotator NewRotation = UKismetMathLibrary::ComposeRotators(MeshComponent->GetComponentRotation(),
		FRotator(LookAxisVector.Y, LookAxisVector.X, 0.0f));
	MeshComponent->SetWorldRotation(NewRotation);
}

void AMPInspectItem::CheckHitResult()
{
	// Get the player controller
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	// Get mouse position
	float MouseX, MouseY;
	if (!PC->GetMousePosition(MouseX, MouseY)) return;
	
	// Get the viewport size
	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
    
	// Convert mouse position to normalized screen coordinates (0-1 range)
	FVector2D NormalizedScreenPos;
	NormalizedScreenPos.X = MouseX / ViewportSizeX;
	NormalizedScreenPos.Y = MouseY / ViewportSizeY;
    
	// Get the Scene Capture 2D component
	if (!SceneCaptureComponent2D) return;
    
	// Create a deprojection matrix from the Scene Capture's perspective
	FVector RayStart, RayEnd;
	if (!RayFromSceneCaptureUV(SceneCaptureComponent2D, NormalizedScreenPos.X, NormalizedScreenPos.Y, 10000.0f, RayStart, RayEnd))
	{
		return;
	}
    
	// Perform the ray trace
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = false;
    
	// Set the collision channel you want to trace against
	ECollisionChannel TraceChannel = ECC_WorldDynamic; // Or your specific channel
    FHitResult Hit;
	
	 bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		RayStart,
		RayEnd,
		TraceChannel,
		CollisionParams
	);

	//TODO: Delete Debug 
	// if (bHit)
	// {
	// 	AActor* DSM= GetWorld()->SpawnActor<AActor>(DebugStaticMesh);
	// 	DSM->SetActorLocation(Hit.ImpactPoint);
	// 	DSM->SetActorRotation(FRotator::ZeroRotator);
	// 	UE_LOG(LogTemp, Warning, TEXT("=================="));
	// 	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.GetActor()->GetActorLabel());
	// 	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint: %s"), *Hit.ImpactPoint.ToString());
	// 	UE_LOG(LogTemp, Warning, TEXT("FaceIndex: %d"), Hit.FaceIndex);
	// 	UE_LOG(LogTemp, Warning, TEXT("Component: %s"), *Hit.Component->GetName());
	// }
	
	if (bHit && Hit.GetComponent() == SkeletalMeshComponent)
	{
		//Interaction with item in inspect vew
		if (Hit.BoneName == LastInspectedItem->SocketName)
		{
			// TODO: Add  Animation or effect
			FName TopicName = "UI.Inventory";
			UEventRouterSubsystem::BroadcastEvent(this, TopicName, FItemUpdatedEvent{LastInspectedItem});
	
			if (UMPItem* ChildItem = LastInspectedItem->ChildItem)
				UEventRouterSubsystem::BroadcastEvent(this, TopicName, FItemPickedEvent{ChildItem});
			
			CloseInspectWidget();
		}
	}
}

void AMPInspectItem::CloseInspectWidget()
{
	if (!InspectWidget) return;
	LastInspectedItem->ItemRotation = GetCurrentMeshComponent()->GetComponentRotation();
	InspectWidget->OnCloseByBackKeyDelegate.RemoveDynamic(this, &AMPInspectItem::CloseInspectWidget);
	InspectWidget->RemoveFromParent();
	bIsInspecting = false;
}

void AMPInspectItem::Zoom(float Value)
{
	float NewFOV = FMath::Clamp(SceneCaptureComponent2D->FOVAngle - Value* 5.0f, 10.0f, 90.0f);
	SceneCaptureComponent2D->FOVAngle = NewFOV;
}

USceneComponent* AMPInspectItem::GetCurrentMeshComponent() const
{
	USceneComponent* CurrentMeshComponent = bIsInteractableItem
		? static_cast<USceneComponent*>(SkeletalMeshComponent)
		: static_cast<USceneComponent*>(StaticMeshComponent);
	return CurrentMeshComponent;
}

bool AMPInspectItem::RayFromSceneCaptureUV(
	USceneCaptureComponent2D* Capture,
	float U, float V,
	float TraceDistance,
	FVector& OutStart,
	FVector& OutEnd)
{
	if (!Capture) return false;

	// Get the scene capture's transform
	FTransform CaptureTransform = Capture->GetComponentTransform();
	FVector CaptureLocation = CaptureTransform.GetLocation();
	FRotator CaptureRotation = CaptureTransform.Rotator();

	// Convert UV (0-1) to NDC (-1 to 1)
	float NDC_X = (U * 2.0f) - 1.0f;
	float NDC_Y = 1.0f - (V * 2.0f); // Flip Y axis

	// Calculate the field of view
	float FOVAngle = Capture->FOVAngle;
	float HalfFOVRadians = FMath::DegreesToRadians(FOVAngle * 0.5f);

	// Calculate aspect ratio from render target
	UTextureRenderTarget2D* RenderTarget = Capture->TextureTarget;
	float AspectRatio = 1.0f;
	if (RenderTarget)
	{
		AspectRatio = static_cast<float>(RenderTarget->SizeX) / static_cast<float>(RenderTarget->SizeY);
	}

	// Calculate ray direction in view space
	float TanHalfFOV = FMath::Tan(HalfFOVRadians);
	FVector ViewSpaceDir;
	ViewSpaceDir.X = 1.0f; // Forward
	ViewSpaceDir.Y = NDC_X * TanHalfFOV * AspectRatio; // Right
	ViewSpaceDir.Z = NDC_Y * TanHalfFOV; // Up

	// Transform to world space
	FVector WorldDirection = CaptureRotation.RotateVector(ViewSpaceDir.GetSafeNormal());

	// Set output ray
	OutStart = CaptureLocation;
	OutEnd = OutStart + (WorldDirection * TraceDistance);

	return true;
}