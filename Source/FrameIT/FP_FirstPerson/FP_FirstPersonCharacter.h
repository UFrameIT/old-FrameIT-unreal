#pragma once

#include "Fact/AngleFact.h"
#include "GameFramework/Character.h"
#include "FP_FirstPersonCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;
class UAnimMontage;
class ASemanticPoint;
class UScrollView;
class UFact;
class UScroll;

UCLASS(config=Game)
class AFP_FirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AFP_FirstPersonCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/* This is when calculating the trace to determine what the weapon has hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponRange;


	/** The current selected weapon.
	 *  - Weapon 0 - Create/Delete Semantic Points
	 *  - Weapon 1 - Measure Distances
	 *  - Weapon 2 - Measure Angles
	 */
	unsigned int WeaponSelected;

	/** The maximum number of weapons. */
	unsigned int MaxNumberOfWeapons;

	bool bInViewMode;

	bool bInSolutionMode;

	int CurrentScrollArrayIndex;

	UPROPERTY()
	UScrollView* CurrentScrollView;

	int CurrentFactIndexSelected;

	UPROPERTY()
	UFact* CurrentFactSelected;

	UPROPERTY()
	UScroll* CurrentScroll;

	int CurrentScrollRequiredFactIndex;

protected:

	int DistanceGunPointsSelected;
	UPROPERTY()
	ASemanticPoint* DistanceGunPoint;

	int AngleGunPointsSelected;
	UPROPERTY()
	ASemanticPoint* AngleGunPointOne;
	UPROPERTY()
	ASemanticPoint* AngleGunPointTwo;

	/** Handler for a touch input beginning. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Fires a virtual projectile with the current weapon in mode one. */
	void OnFireModeOne();

	/** Fires a virtual projectile with the current weapon in mode two. */
	void OnFireModeTwo();

	/** Adds a semantic point and adds it to the registry of facts. */
	void AddSemanticPoint(FVector Location);

	/** Removes a semantic point and removes it from the registry of facts. */
	void RemoveSemanticPoint(FString ID);

	/** Adds a line segement fact to the registry of facts. */
	void AddLineSegmentFact(FString PointOneID, FString PointTwoID, float Distance);

	/** Adds a angle fact to the registry of facts. */
	void AddAngleFact(FString PointOneID, FString PointTwoID, FString PointThreeID, float Angle);

	/** Handle Point Gun Helper*/
	FHitResult HandlePointGunHelper();

	/** Handle Point Gun Mode One i.e. Adding new points*/
	void HandlePointGunModeOne();

	/** Handle Point Gun Mode Two i.e. Removing points*/
	void HandlePointGunModeTwo();

	/** Select Semantic Point - Helper function for Distance and Angle Gun */
	ASemanticPoint* SelectSemanticPoint();

	/** Handle Distance Gun Mode One */
	void HandleDistanceGunModeOne();

	/** Handle Distance Gun Mode Two */
	void HandleDistanceGunModeTwo();

	/** Handle Angle Gun Mode One i.e. Measure the smaller angle*/
	void HandleAngleGunModeOne();

	/** Handle Angle Gun Mode Two i.e. Measure the larger angle*/
	void HandleAngleGunModeTwo();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/** Handles Weapon Mode change */
	void OnWeaponSelectForward();
	
	/** Handles Weapon Mode change */
	void OnWeaponSelectBackward();

	/** Handles Scroll change */
	void OnScrollSelectForward();

	/** Handles Scroll change */
	void OnScrollSelectBackward();

	/** Handles Toggles View mode */
	void OnToggleViewMode();

	/** Handles view assignment down */
	void OnViewModeDown();

	/** Handles view assignment up */
	void OnViewModeUp();

	/** Handles view assignment  */
	void OnViewSelect();

	/** Undo the last action*/
	void ToggleSolutionMode();

	/** Scroll Fact List up*/
	void ScrollFactListUp();

	/** Scroll Fact List down*/
	void ScrollFactListDown();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/* 
	 * Performs a trace between two points
	 * 
	 * @param	StartTrace	Trace starting point
	 * @param	EndTrac		Trace end point
	 * @returns FHitResult returns a struct containing trace result - who/what the trace hit etc.
	 */
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	// Structure that handles touch data so we can process the various stages of touch
	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};

	/*
	 * Handle begin touch event.
	 * Stores the index and location of the touch in a structure
	 *
	 * @param	FingerIndex	The touch index
	 * @param	Location	Location of the touch
	 */
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/*
	 * Handle end touch event.
	 * If there was no movement processed this will fire a projectile, otherwise this will reset pressed flag in the touch structure
	 *
	 * @param	FingerIndex	The touch index
	 * @param	Location	Location of the touch
	 */
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/*
	 * Handle touch update.
	 * This will update the look position based on the change in touching position
	 *
	 * @param	FingerIndex	The touch index
	 * @param	Location	Location of the touch
	 */
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	// Structure to handle touch updating
	TouchData	TouchItem;
	
	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	void TryEnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

