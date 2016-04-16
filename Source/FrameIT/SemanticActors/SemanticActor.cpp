// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "SemanticActor.h"


// Sets default values
ASemanticActor::ASemanticActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASemanticActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASemanticActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FVector ASemanticActor::GetClosestPoint(FVector hitPoint)
{
	return hitPoint;
}
