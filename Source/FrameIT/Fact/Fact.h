// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Fact.generated.h"

/**
 * 
 */
UCLASS(abstract)
class FRAMEIT_API UFact : public UObject
{
	GENERATED_BODY()

public:
	UFact();

	void Initialize(FString id, TArray<UFact*>* LinkFacts = nullptr);

	void ConditionalBeginDestroy();

	void UnlinkFact(UFact* fact);

	virtual void SerializeToMMT() PURE_VIRTUAL(UFact::SerializeToMMT, ;);	


	FString ID;

	TMap<UFact*, UFact*> FactMap;

};
