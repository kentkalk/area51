// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSDefaultInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPSDefaultInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPEFROMAREA51_API IFPSDefaultInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Update health value on widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="In Game HUD")
		void UpdateHUDHealthPercent(const float NewHealthValue);

	// Update energy value on widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="In Game HUD")
		void UpdateHUDEnergyPercent(const float NewEnergyValue);

	// Update clip ammo on widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="In Game HUD")
		void UpdateHUDClipAmmo(const FText& NewClipValue);

	// Update inventory ammo on widget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "In Game HUD")
		void UpdateHUDInvAmmo(const FText& NewInvValue);


	// Get player state from player class to animation blueprints
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player State")
		void GetPlayerAnimState(float& OutPlayerVelocity, bool& bOutIsFalling, bool& bOutHasRifle, bool& bOutIsSwappingWeapons);
};
