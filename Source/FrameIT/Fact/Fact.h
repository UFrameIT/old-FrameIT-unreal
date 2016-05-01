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

	bool Initialize(UWorld* World, FString ID, bool IsScrollFact = false);

	virtual void Destroy();

	virtual void UnlinkFact(UFact* fact);

	virtual void LinkFact(UFact* fact);

	virtual FString SerializeToMMT();
	
	virtual FString SerializeToString();

	unsigned int GetDepth() const;

	FString GetID() const;


	bool IsScrollFact;

protected:
	UPROPERTY()
	FString ID;

	UPROPERTY()
	unsigned int Depth;

	UPROPERTY()
	UWorld* World;

	TMap<UFact*, UFact*> ConnectedFactMap;

};
