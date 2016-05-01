// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ScrollView.generated.h"

class UFact;
class UScroll;
class UPointFact;
class ULineSegmentFact;
class UAngleFact;

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
	void CreateView();


	FString SerializeProofToMMT();
	FString SerializeViewPointFactToMMT(UPointFact* ScrollFact, UPointFact* Fact);
	FString SerializeViewLineSegmentFactToMMT(ULineSegmentFact* ScrollFact, ULineSegmentFact* Fact);
	FString SerializeViewAngleFactToMMT(UAngleFact* ScrollFact, UAngleFact* Fact);



	FString SerializeViewToMMT();

	TMap<int, bool>* GetFactListIndexMap();
	TArray<TPair<UFact*, UFact*>>* GetViewMapping();
	void ResetView();

protected:
	TArray<TPair<UFact*, UFact*>> ViewMapping;

	TMap<int, bool> FactListIndexMap;

	UPROPERTY()
	UScroll* Scroll;

	
};
