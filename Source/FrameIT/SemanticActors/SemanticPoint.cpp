// Fill out your copyright notice in the Description page of Project Settings.

#include "FrameIT.h"
#include "SemanticPoint.h"


ASemanticPoint::ASemanticPoint()
{
	this->SphereRadius = 100.0f;
	
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(this->SphereRadius);
	
	this->PointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointMesh"));
	this->PointMesh->AttachTo(this->RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		this->PointMesh->SetStaticMesh(SphereVisualAsset.Object);
		this->PointMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -this->SphereRadius));
		this->PointMesh->SetWorldScale3D(FVector(2.0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereVisualMaterial(TEXT("/Game/StarterContent/Materials/M_Glass.M_Glass"));
	if (SphereVisualMaterial.Succeeded())
	{
		this->PointMesh->SetMaterial(0, SphereVisualMaterial.Object);
	}


	this->PointLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PointLabel"));
	this->PointLabel->AttachTo(this->RootComponent);
	this->PointLabel->SetRelativeLocation(FVector(this->SphereRadius + 20.0f, this->SphereRadius + 20.0f, this->SphereRadius + 40.0f));
	this->PointLabel->SetHorizontalAlignment(EHTA_Center);
	this->PointLabel->SetWorldSize(150.0f);

}

void ASemanticPoint::SetLabel(FString ID)
{
	this->ID = ID;
	this->PointLabel->SetText(FText::FromString(ID));
}

FString ASemanticPoint::GetLabel()
{
	return this->ID;
}

TPair<bool, FVector> ASemanticPoint::GetClosestPoint(FVector hitPoint)
{
	TPair<bool, FVector> ret;
	ret.Key = true;
	ret.Value = this->GetActorLocation();
	return ret;
}

