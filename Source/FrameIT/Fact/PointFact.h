// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fact/Fact.h"
#include "PointFact.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEIT_API UPointFact : public UFact
{
	GENERATED_BODY()

public:

	virtual FString SerializeToMMT();
	
	virtual FString SerializeToString();

private:

};
