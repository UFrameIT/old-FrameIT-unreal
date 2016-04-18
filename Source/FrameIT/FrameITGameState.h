// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fact/Fact.h"
#include "GameFramework/GameState.h"
#include "FrameITGameState.generated.h"


/**
 * 
 */
UCLASS()
class FRAMEIT_API AFrameITGameState : public AGameState
{
	GENERATED_BODY()

public:
	AFrameITGameState();
	TArray<FText> CreateFactTextList();

	TMap<FString, UFact*> FactMap;
};
