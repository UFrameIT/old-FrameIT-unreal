// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fact/Fact.h"
#include "LineSegmentFact.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEIT_API ULineSegmentFact : public UFact
{
	GENERATED_BODY()

public:
	virtual void SerializeToMMT();

	virtual FString SerializeToString();
	
};
