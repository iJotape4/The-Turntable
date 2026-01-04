// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTInspectItem.generated.h"

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
	UPointLightComponent* PointLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	TSubclassOf<class UTTInspectWidget> InspectWidgetClass;
	
	// Sets default values for this actor's properties
	ATTInspectItem();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void InspectItem(UStaticMesh* Mesh, const FText& ItemName, const FText& ItemDescription = FText::GetEmpty());
};
