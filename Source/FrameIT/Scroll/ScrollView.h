// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Runtime/Online/HTTP/Public/Http.h"
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
	void Initialize(UScroll* Scroll, UWorld* World);

	bool AssignFact(int FactListIndex, UFact* Fact, int RequiredFactIndex);

	TPair<bool, UFact*> ComputeNewFact();

	void CallMMTPostRequest(const FString& SituationTheoryStr, const FString& ViewStr);
	FString CreateSituationTheory();
	FString CreateView();

	void ParseMMT(FString Input);

	FString SerializeProofToMMT();
	FString SerializeViewPointFactToMMT(UPointFact* ScrollFact, UPointFact* Fact);
	FString SerializeViewLineSegmentFactToMMT(ULineSegmentFact* ScrollFact, ULineSegmentFact* Fact);
	FString SerializeViewAngleFactToMMT(UAngleFact* ScrollFact, UAngleFact* Fact);



	FString SerializeViewToMMT();

	TMap<int, bool>* GetFactListIndexMap();
	TArray<TPair<UFact*, UFact*>>* GetViewMapping();
	void ResetView();

	// HTTP things
	void OnMMTPostRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnMMTGetRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnHTTPError(FString ErrorStr);

protected:
	TArray<TPair<UFact*, UFact*>> ViewMapping;

	TMap<int, bool> FactListIndexMap;

	UPROPERTY()
	UScroll* Scroll;

	UPROPERTY()
	UWorld* World;
	
	UPROPERTY()
	FString	MMTServerPostPath;

	UPROPERTY()
	FString	MMTServerGetPath;
};
