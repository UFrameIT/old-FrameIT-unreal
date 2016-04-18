// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "LineSegmentFact.h"
#include "PointFact.h"
#include "FrameITGameState.h"
#include "FrameITGameMode.h"

void ULineSegmentFact::Initialize(FString ID, UPointFact* PointA, UPointFact* PointB, float Distance)
{
	Super::Initialize(ID);

	this->PointA = PointA;
	this->PointB = PointB;

	this->LinkFact((UFact*)PointA);
	this->LinkFact((UFact*)PointB);

	this->Distance = Distance;

}

void ULineSegmentFact::ConditionalBeginDestroy()
{
	Super::ConditionalBeginDestroy();

	this->UnlinkFact((UFact*)PointA);
	this->UnlinkFact((UFact*)PointB);
}

void ULineSegmentFact::UnlinkFact(UFact* fact)
{
	// Delete the reference to the object and unlink ourself from the other point!
	// In other words we are starting to delete ourself as a distance makes no sense with
	// only one point
	if (fact == (UFact*)this->PointA)
	{
		this->PointA = nullptr;
		this->PointB->UnlinkFact(this);
		this->PointB = nullptr;
	}
	else if (fact == (UFact*)this->PointB)
	{
		this->PointB = nullptr;
		this->PointA->UnlinkFact(this);
		this->PointA = nullptr;
	}
	else
	{
		UE_LOG(FrameITLog, Error, TEXT("Fact is not part of Line Segment!"));
		return;
	}

	// Get the current Game State
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return;
	}

	TMap<FString, UFact*>* FactMap = &CurrentGameState->FactMap;
	FactMap->Remove(this->ID);
	CurrentGameMode->OnUpdateFactList(CurrentGameState->CreateFactTextList());
}

void ULineSegmentFact::SerializeToMMT()
{

}

FString ULineSegmentFact::SerializeToString()
{
	return "LineSegment: " + this->ID + " = " + FString::SanitizeFloat(this->Distance);
}
