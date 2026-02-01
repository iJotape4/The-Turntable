// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTInspectItem.generated.h"

class UTTItem;
class UPointLightComponent;

UCLASS()
class THE_TURNATABLE_API ATTInspectItem : public AActor
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
	UStaticMeshComponent* InteractableChildMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UPointLightComponent* PointLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class UTTInspectWidget> InspectWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	UTTInspectWidget* InspectWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	UTTItem* LastInspectedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	TSubclassOf<AActor> DebugStaticMesh;
	
	// Sets default values for this actor's properties
	ATTInspectItem();
	
	void InspectItem(UTTItem* Item);
	
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
	bool bIsInspecting =false;
	// Called when the game starts or when spawned
};
