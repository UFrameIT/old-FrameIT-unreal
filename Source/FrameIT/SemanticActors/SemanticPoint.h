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

	void SetLabel(FString ID);

	FString GetLabel();

	TPair<bool, FVector> GetClosestPoint(FVector hitPoint);
	
private:
	UStaticMeshComponent* PointMesh;
	UTextRenderComponent* PointLabel;
	float SphereRadius;
	FString ID;
};

