// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fact/Fact.h"
#include "AngleFact.generated.h"

class UPointFact;

/**
 * 
 */
UCLASS()
class FRAMEIT_API UAngleFact : public UFact
{
	GENERATED_BODY()

public:
	void Initialize(FString ID, UPointFact* PointA, UPointFact* PointB, UPointFact* PointC, float Angle);

	void ConditionalBeginDestroy();

	virtual void UnlinkFact(UFact* fact);

	virtual void SerializeToMMT();

	virtual FString SerializeToString();

private:
	// Three PointFact
	UPointFact* PointA;
	UPointFact* PointB;
	UPointFact* PointC;

	// The Angle
	float Angle;
};
