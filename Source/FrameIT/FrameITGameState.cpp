// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "FrameITGameState.h"
#include "Fact/Fact.h"
#include "Scroll/Scroll.h"
#include "Fact/PointFact.h"
#include "Fact/LineSegmentFact.h"
#include "Fact/AngleFact.h"


AFrameITGameState::AFrameITGameState()
{
	// Empty Scroll as deleting the widget was too much work and it is long after midnight
	// @Todo Future Self Remove this, definitly maybe, yes
	UScroll* Scroll0 = NewObject<UScroll>(UScroll::StaticClass());
	Scroll0->Initialize(FText::FromString(""), TArray<UFact*>());
	this->ScrollArray.Add(Scroll0);

	// Scroll hard coded!
	//     C
	//   / |
	//  /  | 
	// A---B
	//
	UPointFact* PointAFact = NewObject<UPointFact>(UPointFact::StaticClass());
	PointAFact->Initialize(nullptr, "a", true);
	UPointFact* PointBFact = NewObject<UPointFact>(UPointFact::StaticClass());
	PointBFact->Initialize(nullptr, "b", true);
	UPointFact* PointCFact = NewObject<UPointFact>(UPointFact::StaticClass());
	PointCFact->Initialize(nullptr, "c", true);

	ULineSegmentFact* LineAB = NewObject<ULineSegmentFact>(ULineSegmentFact::StaticClass());
	LineAB->Initialize(nullptr, "a-b", PointAFact, PointBFact, 0, true);

	UAngleFact* AngleABC = NewObject<UAngleFact>(UAngleFact::StaticClass());
	AngleABC->Initialize(nullptr, "a-b-c", PointAFact, PointBFact, PointCFact, 0, true);

	UAngleFact* AngleCAB = NewObject<UAngleFact>(UAngleFact::StaticClass());
	AngleCAB->Initialize(nullptr, "c-a-b", PointCFact, PointAFact, PointBFact, 0, true);

	TArray<UFact*> ScrollFacts;
	ScrollFacts.Add(PointAFact);
	ScrollFacts.Add(PointBFact);
	ScrollFacts.Add(PointCFact);
	ScrollFacts.Add(LineAB);
	ScrollFacts.Add(AngleABC);
	ScrollFacts.Add(AngleCAB);

	FText ScrollText = FText::FromString("Find three points a, b, c, which form the triangle abc, such that the lines a-b and b-c are perpendicular.\n"
		                                 "Then the length of the line segment b-c is:\n\n"
		                                 "b-c = tan(Alpha) * D\n"
		                                 ",where Alpha = Angle(c-a-b) and D = Length(a-b).");

	UScroll* Scroll1 = NewObject<UScroll>(UScroll::StaticClass());
	Scroll1->Initialize(ScrollText, ScrollFacts);	
	this->ScrollArray.Add(Scroll1);

	// Future Scrolls
	UScroll* Scroll2 = NewObject<UScroll>(UScroll::StaticClass());
	Scroll2->Initialize(FText::FromString("Future Scroll 1"), TArray<UFact*>());
	this->ScrollArray.Add(Scroll2);

	UScroll* Scroll3 = NewObject<UScroll>(UScroll::StaticClass());
	Scroll3->Initialize(FText::FromString("Future Scroll 2"), TArray<UFact*>());
	this->ScrollArray.Add(Scroll3);

	this->RequestInProgress = false;

}

void AFrameITGameState::SetRequestInProgress(bool value)
{
	this->RequestInProgress = value;
}

bool AFrameITGameState::GetRequestInProgress()
{
	return this->RequestInProgress;
}

TArray<FText> AFrameITGameState::CreateFactTextList()
{
	TArray<FText> retArr;
	retArr.Reserve(this->FactMap.Num());

	this->FactMap.ValueSort([](UFact& A, UFact& B) {
		if (A.GetDepth() < B.GetDepth())
		{
			return true;
		}
		else if (A.GetDepth() > B.GetDepth())
		{
			return false;
		}
		else
		{
			return A.SerializeToString() < B.SerializeToString();
		}
	});

	for (auto& e : this->FactMap)
	{
		retArr.Add(FText::FromString(e.Value->SerializeToString()));
	}

	return retArr;
}

UFact* AFrameITGameState::GetFact(int Index)
{
	this->FactMap.ValueSort([](UFact& A, UFact& B) {
		if (A.GetDepth() < B.GetDepth())
		{
			return true;
		}
		else if (A.GetDepth() > B.GetDepth())
		{
			return false;
		}
		else
		{
			return A.SerializeToString() < B.SerializeToString();
		}
	});

	// Why do we have no index select function argh...
	int count = 0;
	for (auto& e : this->FactMap)
	{
		if (count == Index)
		{
			return e.Value;
		}
		count++;
	}

	return nullptr;
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
			ID.ReverseString();

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

TArray<UScroll*>* AFrameITGameState::GetScrollArray()
{
	return &this->ScrollArray;
}
