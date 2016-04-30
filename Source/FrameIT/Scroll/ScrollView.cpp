// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "ScrollView.h"
#include "Fact/Fact.h"
#include "Scroll/Scroll.h"
#include "Fact/PointFact.h"
#include "Fact/LineSegmentFact.h"
#include "Fact/AngleFact.h"


UScrollView::UScrollView()
{
	this->Scroll = nullptr;
}

void UScrollView::Initialize(UScroll* Scroll)
{
	this->Scroll = Scroll;
}

bool UScrollView::AssignFact(int FactListIndex, UFact* Fact, int RequiredFactIndex)
{
	if (this->Scroll == nullptr)
	{
		return false;
	}

	// Get Fact from scroll
	auto ScrollFactArray = this->Scroll->GetRequiredFacts();
	if (ScrollFactArray->Num() == 0 || RequiredFactIndex >= ScrollFactArray->Num())
	{
		return false;
	}
	auto ScrollFact = (*ScrollFactArray)[RequiredFactIndex];

	// Check if we assigned the fact already
	if (this->FactListIndexMap.Contains(FactListIndex))
	{
		return false;
	}

	// Here should be one more check i.e. if it is sensible to assign it
	// i.e. check that facts like angle are assigned to the corresponding angle i.e. 
	// A -> a
	// B -> b
	// C -> c
	// Angle A-B-C to Angle a-b-c and not to Angle b-a-c
	// also check if the class matches

	// Add our fact
	TPair<UFact*, UFact*> FactPair;
	FactPair.Key = ScrollFact;
	FactPair.Value = Fact;
	this->ViewMapping.Add(FactPair);
	this->FactListIndexMap.Add(FactListIndex, true);

	return true;
}

TPair<bool, UFact*> UScrollView::ComputeNewFact()
{
	TPair<bool, UFact*> ResultPair;
	ResultPair.Key = false;
	ResultPair.Value = nullptr;

	// Safety check for size
	if (this->ViewMapping.Num() != this->Scroll->GetRequiredFacts()->Num())
	{
		return ResultPair;
	}

	// Create the situation theory
	this->CreateSituationTheory();

	// Create View here a  omdoc file


	// Create a new MMT process and get output
	FString OutputString;
	if (!CallMMT(&OutputString))
	{
		return ResultPair;
	}

	// Parse it
	auto NewFactArray = this->Scroll->ParseMMT(&OutputString);


	return ResultPair;
}

TMap<int, bool>* UScrollView::GetFactListIndexMap()
{
	return &this->FactListIndexMap;
}

TArray<TPair<UFact*, UFact*>>* UScrollView::GetViewMapping()
{
	return &this->ViewMapping;
}

void UScrollView::ResetView()
{
	this->Scroll = nullptr;
	this->ViewMapping.Empty();
	this->FactListIndexMap.Empty();
}

bool UScrollView::CallMMT(FString* OutputString)
{
	// This is windows specific and retarded
	system("C:\\Users\\rocha\\Documents\\MMT\\system\\deploy\\FrameIT.bat > C:\\Users\\rocha\\Desktop\\aaa.xml");

	FString MMTPath = "D:\\Apps\\MinGW\\msys\\1.0\\bin\\cat.exe";
	FString MMTParams = "C:\\Users\\rocha\\Desktop\\aaa.xml";
	int32 OutReturnCode;
	FString OutStdOut;
	FString OutStdErr;

	if (!FPlatformProcess::ExecProcess(*MMTPath, *MMTParams, &OutReturnCode, &OutStdOut, &OutStdErr))
	{
		UE_LOG(FrameITLog, Log, TEXT("Executing MMT process failed!"));
		return false;
	}

	if (OutReturnCode != 0)
	{
		return false;
	}

	UE_LOG(FrameITLog, Log, TEXT("Return Code: %d"), OutReturnCode);
	UE_LOG(FrameITLog, Log, TEXT("StdOut: \n %s"), *OutStdOut);
	UE_LOG(FrameITLog, Log, TEXT("StdErr: \n %s"), *OutStdErr);

	// Remove unncessary output at the start
	int FirstNodePos = OutStdOut.Find("<");
	*OutputString = OutStdOut.RightChop(FirstNodePos);
	
	return true;
}

void UScrollView::CreateSituationTheory()
{
	// Create the content 
	FString Document;	
	FString OmdocHeader = "<omdoc xmlns=\"http://omdoc.org/ns\" xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"    <theory name=\"situation_theory\" base=\"http://cds.omdoc.org/FrameIT\" meta=\"http://cds.omdoc.org/FrameIT?planar_geometry\">\n";
	FString OmdocFooter = "    </theory>\n</omdoc>\n";
	
	Document = OmdocHeader;
	for (auto& e : this->ViewMapping)
	{
		Document += e.Value->SerializeToMMT();
	}
	Document += this->SerializeProofToMMT();
	Document += OmdocFooter;

	// Save the big string
	FString SaveDirectory = FString("C://Users//rocha//Documents//content//FrameIT//content//http..cds.omdoc.org//FrameIT");
	FString FileName = FString("a.xml");
	bool AllowOverwriting = false;

	// Get absolute file path
	FString AbsoluteFilePath = SaveDirectory + "/" + FileName;
	FFileHelper::SaveStringToFile(Document, *AbsoluteFilePath);
}

FString UScrollView::SerializeProofToMMT()
{
	// This is hardcoded for our one scroll as scrolls do not exist yet so it is hard to tell how this exactly should work
	// I guess we need some more meta information which is generated
	auto pA = this->ViewMapping[0].Value;
	auto pB = this->ViewMapping[1].Value;
	auto pC = this->ViewMapping[2].Value;
	auto angleABC = (UAngleFact*)this->ViewMapping[4].Value;
	if (angleABC == nullptr)
	{
		return "";
	}
	FString strPA = "p" + pA->GetID();
	FString strPB = "p" + pB->GetID();
	FString strPC = "p" + pC->GetID();
	FString AngleName = "anglep" + angleABC->PointA->GetID() + "p" + angleABC->PointB->GetID() + "p" + angleABC->PointC->GetID();

	FString ResStr =
		"<constant name=\"givenProof\">\n"
		"    <type>\n"
		"        <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"            <om:OMA>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\"></om:OMS>\n"
		"                <om:OMA>\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"perp\"></om:OMS>\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"pair\"></om:OMS>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                        </om:OMA>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                        </om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + strPA + "\">\n"
		"                        </om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + strPB + "\">\n"
		"                        </om:OMS>\n"
		"                    </om:OMA>\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"pair\"></om:OMS>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                        </om:OMA>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                        </om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + strPB + "\">\n"
		"                        </om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + strPC + "\">\n"
		"                        </om:OMS>\n"
		"                    </om:OMA>\n"
		"                </om:OMA>\n"
		"            </om:OMA>\n"
		"        </om:OMOBJ>\n"
		"    </type>\n"
		"    <definition>\n"
		"        <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"            <om:OMA>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"perp_axiom1\"></om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + pA->GetID() + "\">\n"
		"                </om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + pB->GetID() + "\">\n"
		"                </om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + "p" + pC->GetID() + "\">\n"
		"                </om:OMS>\n"
		"                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + AngleName + "\">\n"
		"                </om:OMS>\n"
		"            </om:OMA>\n"
		"        </om:OMOBJ>\n"
		"    </definition>\n"
		"</constant>\n";

	return ResStr;
}

FString UScrollView::SerializeViewToMMT()
{
	return "";
}

