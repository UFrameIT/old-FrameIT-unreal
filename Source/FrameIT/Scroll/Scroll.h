// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Scroll.generated.h"

class UFact;

/**
 * 
 */
UCLASS()
class FRAMEIT_API UScroll : public UObject
{
	GENERATED_BODY()
	
	
public:
	void Initialize(FText ScrollText, TArray<UFact*> RequiredFacts);

	FText GetScrollText();
protected:
	FText ScrollText;
	TArray<UFact*> RequiredFacts;
	
};
