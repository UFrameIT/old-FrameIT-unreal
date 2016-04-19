// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/GameState.h"
#include "FrameITGameState.generated.h"

class UFact;


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
	FString GetNextFreeName();

	TMap<FString, UFact*>* GetFactMap();

protected:
	TMap<FString, UFact*> FactMap;
};
