// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fact/Fact.h"
#include "LineSegmentFact.generated.h"

class UPointFact;

/**
 * 
 */
UCLASS()
class FRAMEIT_API ULineSegmentFact : public UFact
{
	GENERATED_BODY()

public:
	bool Initialize(UWorld* World, FString ID, UPointFact* PointA, UPointFact* PointB, float Distance, bool IsScrollFact = false);
	
	virtual void Destroy();

	virtual void UnlinkMembers();

	virtual void UnlinkFact(UFact* fact);

	virtual FString SerializeToMMT();

	virtual FString SerializeToString();


	// Two PointFact
	UPROPERTY()
	UPointFact* PointA;

	UPROPERTY()
	UPointFact* PointB;
protected:
	// The Distance
	UPROPERTY()
	float Distance;
};
