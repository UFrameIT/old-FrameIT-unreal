// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "FrameITGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEIT_API AFrameITGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	AFrameITGameMode();

	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "FrameIT")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnUpdateFactList(const TArray<FText>& FactList);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnScrollFactListUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnScrollFactListDown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnWeaponChange(int WeaponMode);

protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrameIT")
	TSubclassOf<UUserWidget> FactWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	UUserWidget* CurrentWidget;
};
