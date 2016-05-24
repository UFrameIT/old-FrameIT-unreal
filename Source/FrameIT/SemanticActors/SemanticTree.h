// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SemanticActors/SemanticActor.h"
#include "SemanticTree.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEIT_API ASemanticTree : public ASemanticActor
{
	GENERATED_BODY()
	
public:
	ASemanticTree();

	TPair<bool, FVector> GetClosestPoint(FVector hitPoint);
private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TreeMesh;
	
	UPROPERTY(EditAnywhere)
	float TreeHeight;	

	UPROPERTY(EditAnywhere)
	float SphereRadiusOfSemPoint;
};
