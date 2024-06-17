// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomDataTypes.h"
#include "FPSPlayerWeapon.generated.h"

UCLASS()
class ESCAPEFROMAREA51_API AFPSPlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSPlayerWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root
	TObjectPtr<UStaticMeshComponent> Root;

public:	
	// Weapon Behavior, set in the blueprint and read by the AFPSPlayer class to understand how the weapon works
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Behavior")
		FWeaponBehavior WeaponBehavior;

	// Visual Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UAnimationAsset> ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UAnimMontage> MeshFiringMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UAnimMontage> MeshReloadMontage;

	// Operation Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Fire(const FTransform CameraWorldTransform, const FVector CameraForwardVector, const bool bInIsCrouched);
		virtual void Fire_Implementation(const FTransform InCameraWorldTransform, const FVector InCameraForwardVector, const bool bInIsCrouched);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Reload();
		virtual void Reload_Implementation();
};
