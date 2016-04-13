// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "FrameIT.h"
#include "FP_FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "StaticMeshResources.h"
#include "SemanticActor.h"

static FName WeaponFireTraceIdent = FName(TEXT("WeaponTrace"));
#define COLLISION_WEAPON		ECC_GameTraceChannel1

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);



// TODO
// Create new bindings for actions
// 2 possible modes measure distance measure / measure angle
//
// state machine inside i.e. fire 2/3 times and each new result point is part of it
// left click -> get direct point / right click -> get semantic point
// 
// create actors to visualize it in the end
// disable simulatneous shots


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
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);

	WeaponRange = 5000.0f;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	// Start with weapon 1
	this->WeaponSelected = 0;
	this->MaxNumberOfWeapons = 3;
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
	// Play a sound if there is one
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
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

	// Check for impact
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
	

	// Deal with impact
	AActor* DamagedActor = Impact.GetActor();
	UPrimitiveComponent* DamagedComponent = Impact.GetComponent();

	const FVector ImpactPoint = Impact.ImpactPoint;
	

	if ((DamagedActor != nullptr) && (DamagedActor != this))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, DamagedActor->GetName());
		UE_LOG(FrameITLog, Log, TEXT("Actor: %s"), *DamagedActor->GetName());
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(DamagedComponent);
		if ((mesh != nullptr) && (mesh->StaticMesh != nullptr) && (mesh->StaticMesh->RenderData != nullptr))
		{
			if (mesh->StaticMesh->RenderData->LODResources.Num() > 0)
			{
				FPositionVertexBuffer* VertexBuffer = &mesh->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;
				if (VertexBuffer)
				{
					const int32 VertexCount = VertexBuffer->GetNumVertices();
					for (int32 Index = 0; Index < VertexCount; Index++)
					{
						//This is in the Static Mesh Actor Class, so it is location and tranform of the SMActor
						const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
						//add to output FVector array
						UE_LOG(FrameITLog, Log, TEXT("i: %d X: %f Y: %f Z: %f"), Index, WorldSpaceVertexLocation.X, WorldSpaceVertexLocation.Y, WorldSpaceVertexLocation.Z);
					}
				}
			}
		}

		ASemanticActor* actor = Cast<ASemanticActor>(DamagedActor);
		if (actor != nullptr) {
			FVector res = actor->GetClosestPoint(ImpactPoint);
			UE_LOG(FrameITLog, Log, TEXT("Semantic Actor: X: %f Y: %f Z: %f"), res.X, res.Y, res.Z);
		}

	}

}

void AFP_FirstPersonCharacter::OnFireModeTwo()
{
	this->OnFireModeOne();
}

void AFP_FirstPersonCharacter::OnWeaponSelectForward()
{
	this->WeaponSelected = (this->WeaponSelected + 1) % this->MaxNumberOfWeapons;
	UE_LOG(FrameITLog, Log, TEXT("OnChangeWeapon forward - New Weapon Selected : %d"), this->WeaponSelected);

}

void AFP_FirstPersonCharacter::OnWeaponSelectBackward()
{
	this->WeaponSelected = (this->WeaponSelected - 1) % this->MaxNumberOfWeapons;
	UE_LOG(FrameITLog, Log, TEXT("OnChangeWeapon backward - New Weapon Selected : %d"), this->WeaponSelected);
}

void AFP_FirstPersonCharacter::UndoLastAction()
{
	UE_LOG(FrameITLog, Log, TEXT("Undo"));
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
