// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "FrameITGameState.h"
#include "Fact/Fact.h"



AFrameITGameState::AFrameITGameState()
{

}

TArray<FText> AFrameITGameState::CreateFactTextList()
{
	TArray<FText> retArr;
	retArr.Reserve(this->FactMap.Num());

	this->FactMap.ValueSort([](UFact& A, UFact& B) {
		return A.GetDepth() < B.GetDepth(); // sort Facts by depth
	});

	for (auto& e : this->FactMap)
	{
		retArr.Add(FText::FromString(e.Value->SerializeToString()));
	}

	return retArr;
}

FString AFrameITGameState::GetNextFreeName()
{
	// Algorithm based on Stack Overflow post:
	// http://stackoverflow.com/questions/18685518/brute-forcer-algorithm-with-all-possible-combinations-permutation-in-a-given-c

	FString ID;

	const unsigned int BaseArrayLength = 26;
	int BaseArray[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
		                  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

	char ResultArray[BaseArrayLength];
	for (int i = 0; i < BaseArrayLength; i++)
	{
		ResultArray[i] = BaseArray[0];
	}
	int IndexArray[BaseArrayLength] = {0};

	bool bFound = false;
	for (int StringLength = 1;
		 StringLength <= BaseArrayLength && bFound == false;
		 StringLength++) {
		
		int UpdateIndex = 0;
		do {
			// check if we found a new unused string
			ID.Empty();
			for (int i = 0; i < StringLength; i++) {
				ID.AppendChar(ResultArray[i]);
			}
			ID.Reverse();

			if (!this->FactMap.Contains(ID))
			{
				bFound = true;
				break;
			}
			
			// update values that need to reset.
			for (UpdateIndex = StringLength - 1;
				UpdateIndex != -1 && ++IndexArray[UpdateIndex] == BaseArrayLength;
				ResultArray[UpdateIndex] = BaseArray[0], IndexArray[UpdateIndex] = 0, UpdateIndex--)
			{

			}

			// update the character that is not resetting, if valid
			if (UpdateIndex != -1)
			{
				ResultArray[UpdateIndex] = BaseArray[IndexArray[UpdateIndex]];
			}
		} while (UpdateIndex != -1);
	}


	return ID;
}

TMap<FString,UFact*>* AFrameITGameState::GetFactMap()
{
	return &this->FactMap;
}

