// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "Scroll.h"
#include "Fact/Fact.h"
#include "Fact/PointFact.h"
#include "Fact/AngleFact.h"
#include "Fact/LineSegmentFact.h"
#include "rapidxml-1.13/rapidxml.hpp"

void UScroll::Initialize(FText ScrollText, TArray<UFact*> RequiredFacts)
{
	this->ScrollText = ScrollText;
	this->RequiredFacts = RequiredFacts;
}

TArray<UFact*> UScroll::ParseMMT(FString* Input)
{
	TArray<UFact*> ResultArray;

	using namespace rapidxml;
	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(TCHAR_TO_ANSI(*(*Input)));    // 0 means default parse flags

	UE_LOG(FrameITLog, Log, TEXT("Name of my first node is: %s"), *FString(doc.first_node()->name()));


	return ResultArray;
}

FText UScroll::GetScrollText()
{
	return this->ScrollText;
}

TArray<UFact*>* UScroll::GetRequiredFacts()
{
	return &this->RequiredFacts;
}