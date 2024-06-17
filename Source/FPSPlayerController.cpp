// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "FPSDefaultInterface.h"

// Constructor
AFPSPlayerController::AFPSPlayerController()
{
}

void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Initialize Enhanced Input Subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	// Select mapping context
	Subsystem->AddMappingContext(MappingContext, 0);
	
	// Get Input Component
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	// Bind Actions

}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InGameHUDClass))
	{
		InGameHUDWidget = Cast<UUserWidget>(CreateWidget(this, InGameHUDClass));
		InGameHUDWidget->AddToViewport();
	}
	
	/* Debug message: If InGameHUDWidget does not implement the correct interface, game will crash whenever accessed as these functions are used 
	   in game ticks without confirming every tick.  This is here to warn of that possibility if launched without the interface */
	if (!InGameHUDWidget->Implements<UFPSDefaultInterface>()) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 120.f, FColor::Red, TEXT("AFPSPlayerController Warning: InGameHUDWidget does not implement UFPSDefaultInterface. Crash likely!"));
	}
}