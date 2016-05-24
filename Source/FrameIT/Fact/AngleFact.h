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
	bool Initialize(UWorld* World, FString ID, UPointFact* PointA, UPointFact* PointB, UPointFact* PointC, float Angle, bool IsScrollFact = false);

	virtual void Destroy();

	virtual void UnlinkMembers();

	virtual void UnlinkFact(UFact* fact);

	virtual FString SerializeToMMT();

	virtual FString SerializeToString();


	// Three PointFact
	UPROPERTY()
	UPointFact* PointA;

	UPROPERTY()
	UPointFact* PointB;

	UPROPERTY()
	UPointFact* PointC;
private:
	// The Angle
	UPROPERTY()
	float Angle;
};
