﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	this->CreateView();

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
	// @todo Remove this hardcoded path!
	// Delete our old file if it exists
	FString AbsoluteFilePath = "C:\\Users\\rocha\\Documents\\content\\pushout.xml";
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*AbsoluteFilePath);


	// This is windows specific and retarded
	WinExec("C:\\Users\\rocha\\Documents\\MMT\\system\\deploy\\FrameIT.bat", SW_HIDE);


	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFilePath))
	{
		return false;
	}

	FFileHelper::LoadFileToString(*OutputString, *AbsoluteFilePath);

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
	FString FileName = FString("situation_theory.omdoc");

	// Get absolute file path
	FString AbsoluteFilePath = SaveDirectory + "/" + FileName;
	FFileHelper::SaveStringToFile(Document, *AbsoluteFilePath);
}

void UScrollView::CreateView()
{
	// Create the content 
	FString Document = this->SerializeViewToMMT();

	// Save the big string
	FString SaveDirectory = FString("C://Users//rocha//Documents//content//FrameIT//content//http..cds.omdoc.org//FrameIT");
	FString FileName = FString("situation_problem_view.omdoc");

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

FString UScrollView::SerializeViewPointFactToMMT(UPointFact* ScrollFact, UPointFact* Fact)
{
	FString ScrollFactID = "P" + ScrollFact->GetID();
	FString FactID = "p" + Fact->GetID();

	FString ResStr =
		"        <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollFactID + "\">\n"
		"            <type>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"Vec3D\"></om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </type>\n"
		"            <definition>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactID + "\">\n"
		"                    </om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </definition>\n"
		"        </constant>\n";

	return ResStr;
}

FString UScrollView::SerializeViewLineSegmentFactToMMT(ULineSegmentFact* ScrollFact, ULineSegmentFact* Fact)
{
	FString FactStrPFirst = "p" + Fact->PointA->GetID();
	FString FactStrPSecond = "p" + Fact->PointB->GetID();
	FString ScrollLineName = "lineSegP" + ScrollFact->PointA->GetID() + "P" + ScrollFact->PointB->GetID();
	FString FactLineName = "lineSegp" + Fact->PointA->GetID() + "p" + Fact->PointB->GetID();


	FString ResStr =
		"        <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollLineName + "_value\">\n"
		"            <type>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"realSym\"></om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </type>\n"
		"            <definition>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactLineName + "_value\">\n"
		"                    </om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </definition>\n"
		"        </constant>\n"
		"        <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollLineName + "\">\n"
		"            <type>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\"></om:OMS>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"equal\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"lineSegmentLength\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPFirst + "\">\n"
		"                                </om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPSecond +"\">\n"
		"                                </om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactLineName + "_value\">\n"
		"                            </om:OMS>\n"
		"                        </om:OMA>\n"
		"                    </om:OMA>\n"
		"                </om:OMOBJ>\n"
		"            </type>\n"
		"            <definition>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactLineName + "\">\n"
		"                    </om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </definition>\n"
		"        </constant>\n";

	return ResStr;
}

FString UScrollView::SerializeViewAngleFactToMMT(UAngleFact* ScrollFact, UAngleFact* Fact)
{
	FString FactStrPFirst = "p" + Fact->PointA->GetID();
	FString FactStrPSecond = "p" + Fact->PointB->GetID();
	FString FactStrPThird = "p" + Fact->PointC->GetID();
	FString ScrollAngleName = "angleP" + ScrollFact->PointA->GetID() + "P" + ScrollFact->PointB->GetID() + "P" + ScrollFact->PointC->GetID();
	FString FactAngleName = "anglep" + Fact->PointA->GetID() + "p" + Fact->PointB->GetID() + "p" + Fact->PointC->GetID();

	FString ResStr =
		"     <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollAngleName + "_value\">\n"
		"       <type><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"realSym\"></om:OMS></om:OMOBJ></type>\n"
		"       <definition><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactAngleName + "_value\"></om:OMS></om:OMOBJ></definition>\n"
		"       \n"
		"     </constant><constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollAngleName + "\">\n"
		"       <type><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\"></om:OMS><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"equal\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS><om:OMA>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"              <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"angle\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPFirst + "\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPSecond + "\"></om:OMS><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPThird + "\"></om:OMS>\n"
		"      </om:OMA><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactAngleName + "_value\"></om:OMS>\n"
		"      </om:OMA>\n"
		"      </om:OMA></om:OMOBJ></type>\n"
		"       <definition><om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\"><om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactAngleName + "\"></om:OMS></om:OMOBJ></definition>\n"
		"       \n"
		"     </constant>\n";
	return ResStr;

	/*
	FString ResStr =
		"        <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollAngleName + "_value\">\n"
		"            <type>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"realSym\"></om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </type>\n"
		"            <definition>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactAngleName + "_value\">\n"
		"                    </om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </definition>\n"
		"        </constant>\n"
		"        <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/" + ScrollAngleName + "\">\n"
		"            <type>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\"></om:OMS>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"equal\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"angle\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPFirst + "\">\n"
		"                                </om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPSecond + "\">\n"
		"                                </om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactStrPThird + "\">\n"
		"                                </om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactAngleName + "\">\n"
		"                            </om:OMS>\n"
		"                        </om:OMA>\n"
		"                    </om:OMA>\n"
		"                </om:OMOBJ>\n"
		"            </type>\n"
		"            <definition>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + FactAngleName + "\">\n"
		"                    </om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </definition>\n"
		"        </constant>\n";
	*/
}

FString UScrollView::SerializeViewToMMT()
{
	FString ResStr =
		"<omdoc xmlns=\"http://omdoc.org/ns\" xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"    <view from=\"http://cds.omdoc.org/FrameIT?problem_theory\" to=\"http://cds.omdoc.org/FrameIT?situation_theory\" name=\"situation_problem_view\" base=\"http://cds.omdoc.org/FrameIT\">\n";
	
	for (auto& e : this->ViewMapping)
	{
		auto Fact = e.Value;
		auto ScrollFact = e.Key;


		if (Fact->GetClass()->IsChildOf(UPointFact::StaticClass()) && ScrollFact->GetClass()->IsChildOf(UPointFact::StaticClass()))
		{
			ResStr += this->SerializeViewPointFactToMMT((UPointFact*)ScrollFact, (UPointFact*)Fact);
		}
		else if (Fact->GetClass()->IsChildOf(ULineSegmentFact::StaticClass()) && ScrollFact->GetClass()->IsChildOf(ULineSegmentFact::StaticClass()))
		{
			ResStr += this->SerializeViewLineSegmentFactToMMT((ULineSegmentFact*)ScrollFact, (ULineSegmentFact*)Fact);
		}
		else if (Fact->GetClass()->IsChildOf(UAngleFact::StaticClass()) && ScrollFact->GetClass()->IsChildOf(UAngleFact::StaticClass()))
		{
			ResStr += this->SerializeViewAngleFactToMMT((UAngleFact*)ScrollFact, (UAngleFact*)Fact);
		}
		else
		{
			UE_LOG(FrameITLog, Log, TEXT("Invalid view assignment!"));
		}
	}		
		
	// Hardcoded again as this part needs to be defined better by scrolls
	auto pA = "p" + this->ViewMapping[0].Value->GetID();
	auto pB = "p" + this->ViewMapping[1].Value->GetID();
	auto pC = "p" + this->ViewMapping[2].Value->GetID();

	ResStr +=
		"        <constant name=\"[http://cds.omdoc.org/FrameIT?problem_theory]/proof\">\n"
		"            <type>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMA>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"HOL\" name=\"ded\"></om:OMS>\n"
		"                        <om:OMA>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                            <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"perp\"></om:OMS>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"pair\"></om:OMS>\n"
		"                                <om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                    <om:OMA>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                    </om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                </om:OMA>\n"
		"                                <om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                    <om:OMA>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                    </om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                </om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + pA + "\">\n"
		"                                </om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + pB + "\">\n"
		"                                </om:OMS>\n"
		"                            </om:OMA>\n"
		"                            <om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"pair\"></om:OMS>\n"
		"                                <om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                    <om:OMA>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                    </om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                </om:OMA>\n"
		"                                <om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                    <om:OMA>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/urtheories\" module=\"LambdaPi\" name=\"apply\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"cartesianProduct\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                        <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                    </om:OMA>\n"
		"                                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"planar_geometry\" name=\"reals\"></om:OMS>\n"
		"                                </om:OMA>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + pB + "\">\n"
		"                                </om:OMS>\n"
		"                                <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"" + pC + "\">\n"
		"                                </om:OMS>\n"
		"                            </om:OMA>\n"
		"                        </om:OMA>\n"
		"                    </om:OMA>\n"
		"                </om:OMOBJ>\n"
		"            </type>\n"
		"            <definition>\n"
		"                <om:OMOBJ xmlns:om=\"http://www.openmath.org/OpenMath\">\n"
		"                    <om:OMS base=\"http://cds.omdoc.org/FrameIT\" module=\"situation_theory\" name=\"givenProof\">\n"
		"                    </om:OMS>\n"
		"                </om:OMOBJ>\n"
		"            </definition>\n"
		"        </constant>\n"
		"    </view>\n"
		"</omdoc>\n";

	return ResStr;
}
