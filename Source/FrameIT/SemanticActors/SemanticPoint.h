// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SemanticActor.h"
#include "SemanticPoint.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEIT_API ASemanticPoint : public ASemanticActor
{
	GENERATED_BODY()
	
public: 
	
	ASemanticPoint();

	FVector GetClosestPoint(FVector hitPoint);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PointMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextRenderComponent* PointLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius;

	FString ID;
};

