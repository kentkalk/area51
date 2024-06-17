// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSPlayerWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPSPlayerWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPEFROMAREA51_API IFPSPlayerWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void WeaponInitialize(const FName WeaponName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void WeaponStartFiring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void WeaponStopFiring();
};
