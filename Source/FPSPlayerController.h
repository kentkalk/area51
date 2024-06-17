// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

UCLASS()
class ESCAPEFROMAREA51_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayerController();

private:

protected:
	void BeginPlay();

public:
	// Setup Enhanced Input
	virtual void SetupInputComponent() override;

	// Enhanced Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputMappingContext* MappingContext;

	// Enhanced Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		class UInputAction* InputTest;

	// In Game HUD Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UUserWidget> InGameHUDClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
		class UUserWidget* InGameHUDWidget;
};
