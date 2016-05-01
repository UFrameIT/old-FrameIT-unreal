// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "AngleFact.h"
#include "PointFact.h"
#include "FrameITGameState.h"
#include "FrameITGameMode.h"

bool UAngleFact::Initialize(UWorld* World, FString ID, UPointFact* PointA, UPointFact* PointB, UPointFact* PointC, float Angle, bool IsScrollFact)
{
	this->World = World;
	this->ID = ID;

	this->PointA = PointA;
	this->PointB = PointB;
	this->PointC = PointC;

	this->Depth = FMath::Max(PointC->GetDepth(), FMath::Max(PointA->GetDepth(), PointB->GetDepth())) + 1;

	this->LinkFact((UFact*)PointA);
	this->LinkFact((UFact*)PointB);
	this->LinkFact((UFact*)PointC);
	PointA->LinkFact(this);
	PointB->LinkFact(this);
	PointC->LinkFact(this);


	this->Angle = Angle;

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

void UAngleFact::UnlinkMembers()
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

	if (this->PointC != nullptr)
	{
		this->PointC->UnlinkFact(this);
		this->PointC = nullptr;
	}
}

void UAngleFact::Destroy()
{
	this->UnlinkMembers();

	Super::Destroy();
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
	else
	{
		UE_LOG(FrameITLog, Error, TEXT("Fact is not part of Angle!"));
		return;
	}

	Super::Destroy();
}

FString UAngleFact::SerializeToMMT()
{
	FString AngleName = "anglep" + this->PointA->GetID() + "p" + this->PointB->GetID() + "p" + this->PointC->GetID();

	FString RetStr =
		"     <constant name=\"" + AngleName + "_value\">\n"
		"       <type><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"tm\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"      </om:OMA></om:OMOBJ></type>\n"
		"       <definition><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMLIT value=\"" + FString::SanitizeFloat(FMath::RadiansToDegrees(this->Angle)) + "\"><type><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"tm\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"      </om:OMA></type></om:OMLIT></om:OMOBJ></definition>\n"
		"       \n"
		"     </constant>\n"
		"     <constant name=\"" + AngleName + "\">\n"
		"       <type><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\"></om:OMS><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"equal\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"angle\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + this->PointA->GetID() + "\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + this->PointB->GetID() + "\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + this->PointC->GetID() + "\"></om:OMS>\n"
		"      </om:OMA><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + AngleName + "_value\"></om:OMS>\n"
		"      </om:OMA>\n"
		"      </om:OMA></om:OMOBJ></type>\n"
		"       \n"
		"       \n"
		"     </constant>\n";
	return RetStr;

	/* MMT fails to parse beautified omdoc for some weird reason therefore we use the ugly way it puts it out
	FString RetStr =
		"<constant name=\"" + AngleName + "_value\">\n"
		"    <type>\n"
		"        <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"            <om:OMA>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\">\n</om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"tm\">\n</om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\">\n</om:OMS>\n"
		"            </om:OMA>\n"
		"        </om:OMOBJ>\n"
		"    </type>\n"
		"    <definition>\n"
		"        <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"            <om:OMLIT value=\"" + FString::SanitizeFloat(this->Angle) + "\">\n"
		"                <type>\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\">\n</om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"tm\">\n</om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\">\n</om:OMS>\n"
		"                    </om:OMA>\n"
		"                </type>\n"
		"            </om:OMLIT>\n"
		"        </om:OMOBJ>\n"
		"    </definition>\n"
		"</constant>\n"
		"<constant name=\"" + AngleName + "\">\n"
		"    <type>\n"
		"        <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"            <om:OMA>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\">\n</om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\">\n</om:OMS>\n"
		"                <om:OMA>\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\">\n</om:OMS>\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"equal\">\n</om:OMS>\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\">\n</om:OMS>\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\">\n</om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"angle\">\n</om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + this->PointA->GetID() + "\">\n"
		"                        </om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + this->PointB->GetID() + "\">\n"
		"                        </om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + this->PointC->GetID() + "\">\n"
		"                        </om:OMS>\n"
		"                    </om:OMA>\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + AngleName + "_value\">\n"
		"                    </om:OMS>\n"
		"                </om:OMA>\n"
		"            </om:OMA>\n"
		"        </om:OMOBJ>\n"
		"    </type>\n"
		"</constant>\n";
		*/
}

FString UAngleFact::SerializeToString()
{
	return "Angle: " + this->ID + " = " + FString::SanitizeFloat(FMath::RadiansToDegrees(this->Angle));
}

