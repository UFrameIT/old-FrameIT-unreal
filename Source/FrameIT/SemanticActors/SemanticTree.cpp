// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "SemanticTree.h"


ASemanticTree::ASemanticTree()
{
	// Create a dummy root component we can attach things to.
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	this->TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMesh"));
	this->TreeMesh->AttachTo(this->RootComponent);

	this->TreeHeight = 875.0f;

	this->SphereRadiusOfSemPoint = 100.0f;
}

TPair<bool, FVector> ASemanticTree::GetClosestPoint(FVector hitPoint)
{
	TPair<bool, FVector> ret;

	FVector TreeBase = this->GetActorLocation();
	FVector TreeTop = this->GetActorLocation();
	TreeTop.Z += this->TreeHeight;

	if (FVector::Dist(hitPoint, TreeTop) < FVector::Dist(hitPoint, TreeBase) &&
		FVector::Dist(hitPoint, TreeTop) <= this->SphereRadiusOfSemPoint)
	{
		ret.Key = true;
		ret.Value = TreeTop;
	}
	else if (FVector::Dist(hitPoint, TreeTop) >= FVector::Dist(hitPoint, TreeBase) &&
		     FVector::Dist(hitPoint, TreeBase) <= this->SphereRadiusOfSemPoint)
	{
		ret.Key = true;
		ret.Value = TreeBase;
	}
	else
	{
		ret.Key = false;
		ret.Value = FVector::ZeroVector;
	}

	return ret;
}
