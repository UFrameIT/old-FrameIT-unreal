// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "LineSegmentFact.h"
#include "PointFact.h"
#include "FrameITGameState.h"
#include "FrameITGameMode.h"
#include "FrameITGameState.h"
#include "FrameITGameMode.h"

bool ULineSegmentFact::Initialize(UWorld* World, FString ID, UPointFact* PointA, UPointFact* PointB, float Distance, bool IsScrollFact)
{
	this->World = World;
	this->ID = ID;

	this->PointA = PointA;
	this->PointB = PointB;

	this->Depth = FMath::Max(PointA->GetDepth(), PointB->GetDepth()) + 1;

	this->LinkFact((UFact*)PointA);
	this->LinkFact((UFact*)PointB);
	PointA->LinkFact(this);
	PointB->LinkFact(this);
	
	this->Distance = Distance;

	this->IsScrollFact = IsScrollFact;

	if (this->IsScrollFact)
	{
		return true;
	}

	// Get the current Game State and Game Mode
	AFrameITGameState* CurrentGameState = (AFrameITGameState*)this->World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		this->UnlinkMembers();
		return false;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)this->World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		this->UnlinkMembers();
		return false;
	}

	auto FactMap = CurrentGameState->GetFactMap();

	if (FactMap->Contains(this->ID))
	{
		this->UnlinkMembers();
		return false;
	}

	FactMap->Add(this->ID, this);
	CurrentGameMode->OnUpdateFactList(CurrentGameState->CreateFactTextList());

	return true;
}

void ULineSegmentFact::Destroy()
{
	this->UnlinkMembers();

	Super::Destroy();
}

void ULineSegmentFact::UnlinkMembers()
{
	if (this->PointA != nullptr)
	{
		this->PointA->UnlinkFact(this);
		this->PointA = nullptr;
	}

	if (this->PointB != nullptr)
	{
		this->PointB->UnlinkFact(this);
		this->PointB = nullptr;
	}
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

	Super::Destroy();
}

void ULineSegmentFact::SerializeToMMT()
{

}

FString ULineSegmentFact::SerializeToString()
{
	return "LineSegment: " + this->ID + " = " + FString::SanitizeFloat(this->Distance);
}
