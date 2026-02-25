// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MPInspectItem.generated.h"

class UMPItem;
class UPointLightComponent;

UCLASS()
class INSPECTIONFRAMEWORK_API AMPInspectItem : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USceneCaptureComponent2D* SceneCaptureComponent2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UPointLightComponent* PointLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class UMPInspectWidget> InspectWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	UMPInspectWidget* InspectWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	UMPItem* LastInspectedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config|Debug")
	TSubclassOf<AActor> DebugStaticMesh;
	
	// Sets default values for this actor's properties
	AMPInspectItem();
	
	void InspectItem(UMPItem* Item);
	
	bool IsInspecting() const { return bIsInspecting; }
	
	void RotateItem(const FVector2D LookAxisVector) const;

	void CheckHitResult();

	bool RayFromSceneCaptureUV(
	USceneCaptureComponent2D* Capture,
	float U, float V,
	float TraceDistance,
	FVector& OutStart,
	FVector& OutEnd);
	
	UFUNCTION()
	void CloseInspectWidget();
	void Zoom(float Value);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	bool bIsInspecting =false;
	
	bool bIsInteractableItem = false;

	USceneComponent* GetCurrentMeshComponent() const;
	// Called when the game starts or when spawned
};