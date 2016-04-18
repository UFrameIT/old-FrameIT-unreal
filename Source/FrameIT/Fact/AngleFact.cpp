// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "AngleFact.h"
#include "PointFact.h"


void UAngleFact::Initialize(FString ID, UPointFact* PointA, UPointFact* PointB, UPointFact* PointC, float Angle)
{
	Super::Initialize(ID);

	this->PointA = PointA;
	this->PointB = PointB;
	this->PointC = PointC;

	this->LinkFact((UFact*)PointA);
	this->LinkFact((UFact*)PointB);
	this->LinkFact((UFact*)PointC);

	this->Angle = Angle;
}

void UAngleFact::ConditionalBeginDestroy()
{
	Super::ConditionalBeginDestroy();

	this->UnlinkFact((UFact*)PointA);
	this->UnlinkFact((UFact*)PointB);
	this->UnlinkFact((UFact*)PointC);
}

void UAngleFact::UnlinkFact(UFact* fact)
{
	// Delete the reference to the object and unlink ourself from the other two points!
	// In other words we are starting to delete ourself as an angle makes no sense with
	// only two points
	if (fact == (UFact*)this->PointA)
	{
		this->PointA = nullptr;
		this->PointB->UnlinkFact(this);
		this->PointC->UnlinkFact(this);
		this->PointB = nullptr;
		this->PointC = nullptr;
	}
	else if (fact == (UFact*)this->PointB)
	{
		this->PointB = nullptr;
		this->PointA->UnlinkFact(this);
		this->PointC->UnlinkFact(this);
		this->PointA = nullptr;
		this->PointC = nullptr;
	}
	else if (fact == (UFact*)this->PointC)
	{
		this->PointC = nullptr;
		this->PointA->UnlinkFact(this);
		this->PointB->UnlinkFact(this);
		this->PointA = nullptr;
		this->PointB = nullptr;
	}
}

void UAngleFact::SerializeToMMT()
{

}

FString UAngleFact::SerializeToString()
{
	return "Angle: " + this->PointA->ID + "-" + this->PointB->ID + "-" + this->PointC->ID + " = " + this->Angle;
}

