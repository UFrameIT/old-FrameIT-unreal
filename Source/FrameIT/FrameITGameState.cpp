// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "FrameITGameState.h"


AFrameITGameState::AFrameITGameState()
{

}

TArray<FText> AFrameITGameState::CreateFactTextList()
{
	TArray<FText> retArr;
	retArr.Reserve(this->FactMap.Num());

	for (auto& e : this->FactMap)
	{
		retArr.Add(FText::FromString(e.Value->SerializeToString()));
	}

	return retArr;
}
