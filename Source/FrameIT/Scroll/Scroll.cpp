// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "Scroll.h"
#include "Fact/Fact.h"
#include "Fact/PointFact.h"
#include "Fact/AngleFact.h"
#include "Fact/LineSegmentFact.h"

void UScroll::Initialize(FText ScrollText, TArray<UFact*> RequiredFacts)
{
	this->ScrollText = ScrollText;
	this->RequiredFacts = RequiredFacts;
}

FText UScroll::GetScrollText()
{
	return this->ScrollText;
}

TArray<UFact*>* UScroll::GetRequiredFacts()
{
	return &this->RequiredFacts;
}
