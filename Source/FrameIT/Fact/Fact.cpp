// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "Fact.h"


UFact::UFact()
{
	this->ID = TEXT("DefaultID");
}

void UFact::Initialize(FString ID, TArray<UFact*>* LinkFacts)
{
	this->ID = ID;
	if (LinkFacts == nullptr)
	{
		return;
	}

	for (auto& e : *LinkFacts)
	{
		this->LinkFact(e);
	}
}

void UFact::ConditionalBeginDestroy()
{
	UE_LOG(FrameITLog, Log, TEXT("Destorying %s now!"), *this->ID);

	for (auto& e : this->FactMap)
	{
		(e.Value)->UnlinkFact(this);
	}

	this->FactMap.Empty();
}

void UFact::UnlinkFact(UFact* fact)
{
	this->FactMap.Remove(fact);
}

void UFact::LinkFact(UFact* fact)
{
	if (this->FactMap.Contains(fact))
	{
		UE_LOG(FrameITLog, Error, TEXT("FactMap already contains a link to the element!"));
	}
	else
	{
		this->FactMap.Add(fact, fact);
	}
}

FString UFact::SerializeToString()
{
	return "";
}
