// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "Fact.h"


UFact::UFact()
{
	this->ID = TEXT("DefaultID");
}

void UFact::Initialize(FString id, TArray<UFact*>* LinkFacts)
{
	this->ID = ID;
	if (LinkFacts == nullptr)
	{
		return;
	}

	for (auto& e : *LinkFacts)
	{
		if (this->FactMap.Contains(e))
		{
			UE_LOG(FrameITLog, Error, TEXT("FactMap already contains a link to the element!"));
		}
		else
		{
			this->FactMap.Add(e, e);
		}
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
