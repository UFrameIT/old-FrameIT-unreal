// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "Fact.h"
#include "FrameITGameState.h"
#include "FrameITGameMode.h"

UFact::UFact()
{
	this->ID = TEXT("DefaultID");
	this->Depth = 0;
}

bool UFact::Initialize(UWorld* World, FString ID, bool IsScrollFact)
{

	UE_LOG(FrameITLog, Log, TEXT("Initialized Fact %s!"), *ID);

	this->ID = ID;

	this->World = World;

	this->Depth = 0;

	this->IsScrollFact = IsScrollFact;

	if (this->IsScrollFact)
	{
		return true;
	}

	AFrameITGameState* CurrentGameState = (AFrameITGameState*)this->World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return false;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)this->World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return false;
	}

	auto FactMap = CurrentGameState->GetFactMap();
	
	if (FactMap->Contains(this->ID))
	{
		return false;
	}

	FactMap->Add(this->ID, this);
	CurrentGameMode->OnUpdateFactList(CurrentGameState->CreateFactTextList());

	return true;
}

void UFact::Destroy()
{
	UE_LOG(FrameITLog, Log, TEXT("Destorying %s now!"), *this->ID);

	for (auto& e : this->ConnectedFactMap)
	{
		(e.Value)->UnlinkFact(this);
	}

	this->ConnectedFactMap.Empty();

	if (this->IsScrollFact)
	{
		return;
	}

	// Get the current Game State and Game Mode
	AFrameITGameState* CurrentGameState = (AFrameITGameState*)this->World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)this->World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return;
	}

	auto FactMap = CurrentGameState->GetFactMap();

	FactMap->Remove(this->ID);
	CurrentGameMode->OnUpdateFactList(CurrentGameState->CreateFactTextList());
}

void UFact::UnlinkFact(UFact* fact)
{
	this->ConnectedFactMap.Remove(fact);
}

void UFact::LinkFact(UFact* fact)
{
	if (this->ConnectedFactMap.Contains(fact))
	{
		UE_LOG(FrameITLog, Error, TEXT("FactMap already contains a link to the element!"));
	}
	else
	{
		this->ConnectedFactMap.Add(fact, fact);
	}
}

FString UFact::SerializeToString()
{
	return "";
}

FString UFact::SerializeToMMT()
{
	return "";
}

unsigned int UFact::GetDepth() const
{
	return this->Depth;
}

FString UFact::GetID() const
{
	return this->ID;
}
