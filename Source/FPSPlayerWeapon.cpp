// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/DataTable.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

// Constructor
AFPSPlayerWeapon::AFPSPlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Root Component
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = Root;

	// Add Skeletal Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFPSPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the parent wants the weapon to fire
void AFPSPlayerWeapon::Fire_Implementation(const FTransform InCameraWorldTransform, const FVector InCameraForwardVector, const bool bInIsCrouched)
{
	if (IsValid(FireAnimation))
	{
		SkeletalMesh->PlayAnimation(FireAnimation, false);

		// Start at player camera location
		FVector TraceStart = InCameraWorldTransform.GetLocation();

		// Number of pellets increases number of traces run
		for (int i = 0; i < WeaponBehavior.PelletCount; i++)
		{
			// End vector
			FVector DirectionWithSpread;
			// Spread is adjustable to be different when crouched
			if (bInIsCrouched)
			{
				DirectionWithSpread = FMath::VRandCone(InCameraForwardVector, FMath::DegreesToRadians(WeaponBehavior.SpreadIfCrouched));
			}
			else
			{
				DirectionWithSpread = FMath::VRandCone(InCameraForwardVector, FMath::DegreesToRadians(WeaponBehavior.Spread));
			}
			// Weapon range is also adjustable by scaling the vector
			FVector TraceAdjustor = DirectionWithSpread * WeaponBehavior.Range;
			// Add back to the location
			FVector TraceEnd = InCameraWorldTransform.GetLocation() + TraceAdjustor;

			TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

			FHitResult HitResult;
			FCollisionQueryParams TraceParams;
			bool bDidHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

			// Debug Traces
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5.0f);
			if (bDidHit)
			{
				DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5.f, 5.f, 5.f), FColor::Red, false, 5.0f);
			}
		}
	}
}

// Called when the parent wants the weapon to reload
void AFPSPlayerWeapon::Reload_Implementation()
{
}


