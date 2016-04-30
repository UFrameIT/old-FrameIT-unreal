// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ScrollView.generated.h"

class UFact;
class UScroll;

/**
 * 
 */
UCLASS()
class FRAMEIT_API UScrollView : public UObject
{
	GENERATED_BODY()
	
public:
	UScrollView();
	void Initialize(UScroll* Scroll);

	bool AssignFact(int FactListIndex, UFact* Fact, int RequiredFactIndex);

	TPair<bool, UFact*> ComputeNewFact();

	bool CallMMT(FString* OutputString);
	void CreateSituationTheory();
	FString SerializeProofToMMT();
	FString SerializeViewToMMT();

	TMap<int, bool>* GetFactListIndexMap();
	TArray<TPair<UFact*, UFact*>>* GetViewMapping();
	void ResetView();

protected:
	TArray<TPair<UFact*, UFact*>> ViewMapping;

	TMap<int, bool> FactListIndexMap;
	UScroll* Scroll;

	
};
