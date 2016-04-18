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
	void Initialize(FString ID, UPointFact* PointA, UPointFact* PointB, float Distance);
	
	void ConditionalBeginDestroy();

	virtual void UnlinkFact(UFact* fact);

	virtual void SerializeToMMT();

	virtual FString SerializeToString();

protected:
	// Three PointFact
	UPointFact* PointA;
	UPointFact* PointB;

	// The Distance
	float Distance;
};
