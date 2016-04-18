// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "PointFact.h"


void UPointFact::SerializeToMMT()
{

}

FString UPointFact::SerializeToString()
{
	return "Point: " + this->ID;
}
