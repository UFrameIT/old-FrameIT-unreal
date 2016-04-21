// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FrameIT.h"
#include "FP_FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "StaticMeshResources.h"
#include "SemanticActors/SemanticActor.h"
#include "SemanticActors/SemanticPoint.h"
#include "FrameITGameState.h"
#include "FrameITGameMode.h"
#include "Fact/PointFact.h"
#include "Fact/LineSegmentFact.h"
#include "Fact/AngleFact.h"
#include "Scroll/Scroll.h"

static FName WeaponFireTraceIdent = FName(TEXT("WeaponTrace"));
#define COLLISION_WEAPON		ECC_GameTraceChannel1

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);



//////////////////////////////////////////////////////////////////////////
// AFP_FirstPersonCharacter

AFP_FirstPersonCharacter::AFP_FirstPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	WeaponRange = 10000.0f;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	// Start with weapon 1
	this->WeaponSelected = 0;
	this->MaxNumberOfWeapons = 3;

	// Setup Distance Gun
	this->DistanceGunPointsSelected = 0;
	this->DistanceGunPoint = nullptr;

	// Setup Angle Gun
	this->AngleGunPointsSelected = 0;
	this->AngleGunPointOne = nullptr;
	this->AngleGunPointTwo = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFP_FirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	// Bind jump events
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Bind Fire Events
	InputComponent->BindAction("FireModeOne", IE_Pressed, this, &AFP_FirstPersonCharacter::OnFireModeOne);
	InputComponent->BindAction("FireModeTwo", IE_Pressed, this, &AFP_FirstPersonCharacter::OnFireModeTwo);

	// Bind weapon swap events
	InputComponent->BindAction("WeaponSelectForward", IE_Pressed, this, &AFP_FirstPersonCharacter::OnWeaponSelectForward);
	InputComponent->BindAction("WeaponSelectBackward", IE_Pressed, this, &AFP_FirstPersonCharacter::OnWeaponSelectBackward);

	// Bind Fact board scroll events
	InputComponent->BindAction("ScrollFactListDown", IE_Pressed, this, &AFP_FirstPersonCharacter::ScrollFactListDown);
	InputComponent->BindAction("ScrollFactListDown", IE_Released, this, &AFP_FirstPersonCharacter::ScrollFactListDown);
	InputComponent->BindAction("ScrollFactListUp", IE_Pressed, this, &AFP_FirstPersonCharacter::ScrollFactListUp);
	InputComponent->BindAction("ScrollFactListUp", IE_Released, this, &AFP_FirstPersonCharacter::ScrollFactListUp);

	// Bind Scroll select events
	InputComponent->BindAction("ScrollSelectForward", IE_Pressed, this, &AFP_FirstPersonCharacter::OnScrollSelectForward);
	InputComponent->BindAction("ScrollSelectBackward", IE_Pressed, this, &AFP_FirstPersonCharacter::OnScrollSelectBackward);
	InputComponent->BindAction("ToggleViewMode", IE_Pressed, this, &AFP_FirstPersonCharacter::OnToggleViewMode);

	// Bind view mode events
	InputComponent->BindAction("ViewModeDown", IE_Pressed, this, &AFP_FirstPersonCharacter::OnViewModeDown);
	InputComponent->BindAction("ViewModeUp", IE_Pressed, this, &AFP_FirstPersonCharacter::OnViewModeUp);
	InputComponent->BindAction("ViewSelect", IE_Pressed, this, &AFP_FirstPersonCharacter::OnViewSelect);
	

	// Bind Undo Event
	InputComponent->BindAction("UndoLastAction", IE_Pressed, this, &AFP_FirstPersonCharacter::UndoLastAction);
	
	TryEnableTouchscreenMovement(InputComponent);

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &AFP_FirstPersonCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFP_FirstPersonCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFP_FirstPersonCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFP_FirstPersonCharacter::LookUpAtRate);
}

void AFP_FirstPersonCharacter::OnFireModeOne()
{
	switch (this->WeaponSelected)
	{
	case 0:
		UE_LOG(FrameITLog, Log, TEXT("Point Gun fired M2"));
		this->HandlePointGunModeOne();
		break;
	case 1:
		UE_LOG(FrameITLog, Log, TEXT("Distance Gun fired M1"));
		this->HandleDistanceGunModeOne();
		break;
	case 2:
		UE_LOG(FrameITLog, Log, TEXT("Angle Gun fired M1"));
		this->HandleAngleGunModeOne();
		break;
	default:
		UE_LOG(FrameITLog, Error, TEXT("Invalid weapon selected!"));
	}
}

void AFP_FirstPersonCharacter::OnFireModeTwo()
{
	switch (this->WeaponSelected)
	{
	case 0:
		UE_LOG(FrameITLog, Log, TEXT("Point Gun fired M2"));
		this->HandlePointGunModeTwo();
		break;
	case 1:
		UE_LOG(FrameITLog, Log, TEXT("Distance Gun fired M2"));
		this->HandleDistanceGunModeTwo();
		break;
	case 2:
		UE_LOG(FrameITLog, Log, TEXT("Angle Gun fired M2"));
		this->HandleAngleGunModeTwo();
		break;
	default:
		UE_LOG(FrameITLog, Error, TEXT("Invalid weapon selected!"));
	}
}

void AFP_FirstPersonCharacter::AddSemanticPoint(FVector Location)
{
	// Get the current Game State
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return;
	}

	auto FactMap = CurrentGameState->GetFactMap();
	FString ID = CurrentGameState->GetNextFreeName();
	
	// Construct the Fact and add it to the Fact registry
	UPointFact* Fact = NewObject<UPointFact>(UPointFact::StaticClass());
	if (Fact->Initialize(World, ID) == false)
	{
		return;
	}

	ASemanticPoint* SemanticPoint = (ASemanticPoint*)(World->SpawnActor<ASemanticPoint>(Location, FRotator::ZeroRotator));
	SemanticPoint->SetLabel(ID);

}

void AFP_FirstPersonCharacter::RemoveSemanticPoint(FString ID)
{
	// Get the current Game State
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return;
	}

	auto FactMap = CurrentGameState->GetFactMap();
	UPointFact* Fact = (UPointFact*)*FactMap->Find(ID);
	if (Fact != nullptr)
	{
		Fact->Destroy();
	}
}

void AFP_FirstPersonCharacter::AddLineSegmentFact(FString PointOneID, FString PointTwoID, float Distance)
{
	// Get the current Game State
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return;
	}

	auto FactMap = CurrentGameState->GetFactMap();

	auto ResultPointOne = (UPointFact*)*FactMap->Find(PointOneID);
	if (ResultPointOne == nullptr)
	{
		return;
	}

	auto ResultPointTwo = (UPointFact*)*FactMap->Find(PointTwoID);
	if (ResultPointTwo == nullptr)
	{
		return;
	}

	// Construct the Fact and add it to the Fact registry
	ULineSegmentFact* Fact = NewObject<ULineSegmentFact>(ULineSegmentFact::StaticClass());
	FString ID = ResultPointOne->GetID() + "-" + ResultPointTwo->GetID();

	Fact->Initialize(World, ID, ResultPointOne, ResultPointTwo, Distance);
}

void AFP_FirstPersonCharacter::AddAngleFact(FString PointOneID, FString PointTwoID, FString PointThreeID, float Angle)
{
	// Get the current Game State
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	if (CurrentGameState == nullptr)
	{
		return;
	}

	auto FactMap = CurrentGameState->GetFactMap();

	auto ResultPointOne = (UPointFact*)*FactMap->Find(PointOneID);
	if (ResultPointOne == nullptr)
	{
		return;
	}

	auto ResultPointTwo = (UPointFact*)*FactMap->Find(PointTwoID);
	if (ResultPointTwo == nullptr)
	{
		return;
	}

	auto ResultPointThree = (UPointFact*)*FactMap->Find(PointThreeID);
	if (ResultPointThree == nullptr)
	{
		return;
	}

	// Construct the Fact and add it to the Fact registry
	UAngleFact* Fact = NewObject<UAngleFact>(UAngleFact::StaticClass());
	FString ID = ResultPointOne->GetID() + "-" + ResultPointTwo->GetID() + "-" + ResultPointThree->GetID();

	Fact->Initialize(World, ID, ResultPointOne, ResultPointTwo, ResultPointThree, Angle);
}

FHitResult AFP_FirstPersonCharacter::HandlePointGunHelper()
{
	// Play a sound if there is one
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Now send a trace from the end of our gun to see if we should hit anything
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	// Calculate the direction of fire and the start location for trace
	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector ShootDir = CamRot.Vector();

	FVector StartTrace = FVector::ZeroVector;
	if (PlayerController)
	{
		FRotator UnusedRot;
		PlayerController->GetPlayerViewPoint(StartTrace, UnusedRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);
	}

	// Calculate endpoint of trace
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	return WeaponTrace(StartTrace, EndTrace);
}

void AFP_FirstPersonCharacter::HandlePointGunModeOne()
{
	// Check for impact and handle graphics and sound
	const FHitResult Impact = this->HandlePointGunHelper();

	// Deal with impact
	AActor* DamagedActor = Impact.GetActor();

	const FVector ImpactPoint = Impact.ImpactPoint;
	
	if ((DamagedActor != nullptr) && (DamagedActor != this))
	{
		UE_LOG(FrameITLog, Log, TEXT("Actor hit: %s"), *DamagedActor->GetName());

		// if the point is a semantic point then delete it
		ASemanticPoint* SemPoint = Cast<ASemanticPoint>(DamagedActor);
		if (SemPoint != nullptr)
		{
			SemPoint->Destroy();
			this->RemoveSemanticPoint(SemPoint->GetLabel());
			return;
		}
	
		// if it is any other semantic actor then get the closest impact point and spawn a point there
		ASemanticActor* SemActor = Cast<ASemanticActor>(DamagedActor);
		if (SemActor != nullptr)
		{
			auto ClosestPoint = SemActor->GetClosestPoint(ImpactPoint);
			UE_LOG(FrameITLog, Log, TEXT("Semantic Actor Bool: %d -: X: %f Y: %f Z: %f"),
				   ClosestPoint.Key,
				   ClosestPoint.Value.X,
				   ClosestPoint.Value.Y,
				   ClosestPoint.Value.Z);
			if (ClosestPoint.Key)
			{
				this->AddSemanticPoint(ClosestPoint.Value);
			}
		}
	}
}

void AFP_FirstPersonCharacter::HandlePointGunModeTwo()
{
	// Check for impact and handle graphics and sound
	const FHitResult Impact = this->HandlePointGunHelper();

	// Deal with impact
	AActor* DamagedActor = Impact.GetActor();

	const FVector ImpactPoint = Impact.ImpactPoint;

	if ((DamagedActor != nullptr) && (DamagedActor != this))
	{
		UE_LOG(FrameITLog, Log, TEXT("Actor hit: %s"), *DamagedActor->GetName());

		// if the point is a semantic point then delete it
		ASemanticPoint* SemPoint = Cast<ASemanticPoint>(DamagedActor);
		if (SemPoint != nullptr)
		{
			SemPoint->Destroy();
			this->RemoveSemanticPoint(SemPoint->GetLabel());
		}
		else
		{
			// Otherwise add a point at the Impact Point
			this->AddSemanticPoint(ImpactPoint);
		}
	}
}

ASemanticPoint* AFP_FirstPersonCharacter::SelectSemanticPoint()
{
	// Play a sound if there is one
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Now send a trace from the end of our gun to see if we should hit anything
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	// Calculate the direction of fire and the start location for trace
	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector ShootDir = CamRot.Vector();

	FVector StartTrace = FVector::ZeroVector;
	if (PlayerController)
	{
		FRotator UnusedRot;
		PlayerController->GetPlayerViewPoint(StartTrace, UnusedRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);
	}

	// Calculate endpoint of trace
	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	// Check for impact and handle graphics and sound
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// Deal with impact
	AActor* DamagedActor = Impact.GetActor();

	const FVector ImpactPoint = Impact.ImpactPoint;

	ASemanticPoint* SemPoint = nullptr;
	if ((DamagedActor != nullptr) && (DamagedActor != this))
	{
		UE_LOG(FrameITLog, Log, TEXT("Actor: %s"), *DamagedActor->GetName());

		SemPoint = Cast<ASemanticPoint>(DamagedActor);
	}

	return SemPoint;
}

void AFP_FirstPersonCharacter::HandleDistanceGunModeOne()
{
	if (this->DistanceGunPointsSelected == 0)
	{
		this->DistanceGunPoint = SelectSemanticPoint();
		if (this->DistanceGunPoint != nullptr)
		{
			this->DistanceGunPointsSelected = 1;
			UE_LOG(FrameITLog, Log, TEXT("Distance Gun First Point Selected %s"), *this->DistanceGunPoint->GetLabel());
		}
	}
	else
	{
		ASemanticPoint* SemPoint = SelectSemanticPoint();
		if (SemPoint != nullptr)
		{
			// check if we did not measure a vertical vector only allow up to a certain incline
			float MinAngle = 20.0f;
			FVector FirstVec = SemPoint->GetActorLocation() - this->DistanceGunPoint->GetActorLocation();
			FVector SecondVec = FVector::UpVector;
			FirstVec.Normalize();
			SecondVec.Normalize();
			float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(FirstVec, SecondVec)));
			UE_LOG(FrameITLog, Log, TEXT("Distance Point Angle: %f"), Angle);
			if (Angle > MinAngle)
			{
				this->AddLineSegmentFact(this->DistanceGunPoint->GetLabel(),
					SemPoint->GetLabel(),
					FVector::Dist(this->DistanceGunPoint->GetActorLocation(), SemPoint->GetActorLocation()));
			}
			UE_LOG(FrameITLog, Log, TEXT("Distance Gun Second Point Selected %s"), *SemPoint->GetLabel());
		}
		
		this->DistanceGunPointsSelected = 0;
		this->DistanceGunPoint = nullptr;
	}
	
}

void AFP_FirstPersonCharacter::HandleDistanceGunModeTwo()
{
	this->DistanceGunPointsSelected = 0;
	this->DistanceGunPoint = nullptr;
}

void AFP_FirstPersonCharacter::HandleAngleGunModeOne()
{
	if (this->AngleGunPointsSelected == 0)
	{
		this->AngleGunPointOne = SelectSemanticPoint();
		if (this->AngleGunPointOne != nullptr)
		{
			this->AngleGunPointsSelected = 1;
			UE_LOG(FrameITLog, Log, TEXT("Angle Gun First Point Selected %s"), *this->AngleGunPointOne->GetLabel());
		}
	}
	else if (this->AngleGunPointsSelected == 1)
	{
		this->AngleGunPointTwo = SelectSemanticPoint();
		if (this->AngleGunPointTwo != nullptr)
		{
			this->AngleGunPointsSelected = 2;
			UE_LOG(FrameITLog, Log, TEXT("Angle Gun Second Point Selected %s"), *this->AngleGunPointTwo->GetLabel());
		}
	}
	else
	{
		ASemanticPoint* SemPoint = SelectSemanticPoint();
		if (SemPoint != nullptr)
		{
			FVector FirstVec = this->AngleGunPointTwo->GetActorLocation() - this->AngleGunPointOne->GetActorLocation();
			FVector SecondVec = SemPoint->GetActorLocation() - this->AngleGunPointTwo->GetActorLocation();
			FirstVec.Normalize();
			SecondVec.Normalize();
			float Angle = acosf(FVector::DotProduct(FirstVec, SecondVec));
			this->AddAngleFact(this->AngleGunPointOne->GetLabel(),
							   this->AngleGunPointTwo->GetLabel(),
							   SemPoint->GetLabel(),
							   Angle);

			UE_LOG(FrameITLog, Log, TEXT("Angle Gun Third Point Selected %s"), *SemPoint->GetLabel());
		}

		this->AngleGunPointsSelected = 0;
		this->AngleGunPointOne = nullptr;
		this->AngleGunPointTwo = nullptr;
	}
}

void AFP_FirstPersonCharacter::HandleAngleGunModeTwo()
{
	this->AngleGunPointsSelected = 0;
	this->AngleGunPointOne = nullptr;
	this->AngleGunPointTwo = nullptr;
}

void AFP_FirstPersonCharacter::OnWeaponSelectForward()
{
	// Reset Distance Gun
	this->DistanceGunPointsSelected = 0;
	this->DistanceGunPoint = nullptr;

	// Reset Angle Gun
	this->AngleGunPointsSelected = 0;
	this->AngleGunPointOne = nullptr;
	this->AngleGunPointTwo = nullptr;

	this->WeaponSelected = (this->WeaponSelected + 1) % this->MaxNumberOfWeapons;
	UE_LOG(FrameITLog, Log, TEXT("OnChangeWeapon forward - New Weapon Selected : %d"), this->WeaponSelected);

	// Get the current Game Mode and call weapon change event
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return;
	}
	CurrentGameMode->OnWeaponChange(this->WeaponSelected);

}

void AFP_FirstPersonCharacter::OnWeaponSelectBackward()
{
	// Reset Distance Gun
	this->DistanceGunPointsSelected = 0;
	this->DistanceGunPoint = nullptr;

	// Reset Angle Gun
	this->AngleGunPointsSelected = 0;
	this->AngleGunPointOne = nullptr;
	this->AngleGunPointTwo = nullptr;

	if (this->WeaponSelected == 0)
	{
		this->WeaponSelected = this->MaxNumberOfWeapons - 1;
	}
	else
	{
		this->WeaponSelected = (this->WeaponSelected - 1) % this->MaxNumberOfWeapons;
	}
	UE_LOG(FrameITLog, Log, TEXT("OnChangeWeapon backward - New Weapon Selected : %d"), this->WeaponSelected);
	
	// Get the current Game Mode and call weapon change event
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return;
	}
	CurrentGameMode->OnWeaponChange(this->WeaponSelected);

}

void AFP_FirstPersonCharacter::OnScrollSelectForward()
{
	// Get the current Game Mode and call scroll change event
	UWorld* const World = GetWorld();
	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();

	auto ScrollArray = CurrentGameState->GetScrollArray();
	int CurIndex = CurrentGameState->GetCurrentScrollArrayIndex();
	CurIndex = (CurIndex + 1) % ScrollArray->Num();
	CurrentGameState->SetCurrentScrollArrayIndex(CurIndex);
	auto Scroll = (*ScrollArray)[CurIndex];

	CurrentGameMode->OnScrollSelect(Scroll->GetScrollText());
}

void AFP_FirstPersonCharacter::OnScrollSelectBackward()
{
	// Get the current Game Mode and call scroll change event
	UWorld* const World = GetWorld();
	AFrameITGameState* CurrentGameState = (AFrameITGameState*)World->GetGameState();
	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();

	auto ScrollArray = CurrentGameState->GetScrollArray();
	int CurIndex = CurrentGameState->GetCurrentScrollArrayIndex();
	CurIndex--;
	if (CurIndex < 0)
	{
		CurIndex = ScrollArray->Num() - 1;
	}
	CurrentGameState->SetCurrentScrollArrayIndex(CurIndex);
	auto Scroll = (*ScrollArray)[CurIndex];

	CurrentGameMode->OnScrollSelect(Scroll->GetScrollText());
}

void AFP_FirstPersonCharacter::OnToggleViewMode()
{

}

void AFP_FirstPersonCharacter::OnViewModeDown()
{

}

void AFP_FirstPersonCharacter::OnViewModeUp()
{

}

void AFP_FirstPersonCharacter::OnViewSelect()
{

}

void AFP_FirstPersonCharacter::UndoLastAction()
{
	UE_LOG(FrameITLog, Log, TEXT("Undo - Currently test code"));
}

void AFP_FirstPersonCharacter::ScrollFactListUp()
{
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return;
	}

	CurrentGameMode->OnScrollFactListUp();
}

void AFP_FirstPersonCharacter::ScrollFactListDown()
{
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AFrameITGameMode* CurrentGameMode = (AFrameITGameMode*)World->GetAuthGameMode();
	if (CurrentGameMode == nullptr)
	{
		return;
	}

	CurrentGameMode->OnScrollFactListDown();
}

void AFP_FirstPersonCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If touch is already pressed check the index. If it is not the same as the current touch assume a second touch and thus we want to fire
	if (TouchItem.bIsPressed == true)
	{
		if( TouchItem.FingerIndex != FingerIndex)
		{
			OnFireModeOne();
		}
	}
	else 
	{
		// Cache the finger index and touch location and flag we are processing a touch
		TouchItem.bIsPressed = true;
		TouchItem.FingerIndex = FingerIndex;
		TouchItem.Location = Location;
		TouchItem.bMoved = false;
	}
}

void AFP_FirstPersonCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If we didnt record the start event do nothing, or this is a different index
	if((TouchItem.bIsPressed == false) || ( TouchItem.FingerIndex != FingerIndex) )
	{
		return;
	}

	// If the index matches the start index and we didn't process any movement we assume we want to fire
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFireModeOne();
	}

	// Flag we are no longer processing the touch event
	TouchItem.bIsPressed = false;
}

void AFP_FirstPersonCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// If we are processing a touch event and this index matches the initial touch event process movement
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				FVector MoveDelta = Location - TouchItem.Location;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
				if (FMath::Abs(ScaledDelta.X) >= (4.0f / ScreenSize.X))
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.X * BaseTurnRate;
					AddControllerYawInput(Value);
				}
				if (FMath::Abs(ScaledDelta.Y) >= (4.0f / ScreenSize.Y))
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.Y* BaseTurnRate;
					AddControllerPitchInput(Value);
				}
				TouchItem.Location = Location;
			}
			TouchItem.Location = Location;
		}
	}
}

void AFP_FirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFP_FirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFP_FirstPersonCharacter::TurnAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFP_FirstPersonCharacter::LookUpAtRate(float Rate)
{
	// Calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFP_FirstPersonCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(WeaponFireTraceIdent, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void AFP_FirstPersonCharacter::TryEnableTouchscreenMovement(UInputComponent* InputComponent)
{
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFP_FirstPersonCharacter::BeginTouch);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &AFP_FirstPersonCharacter::EndTouch);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFP_FirstPersonCharacter::TouchUpdate);	
}
