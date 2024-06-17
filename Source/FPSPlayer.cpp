// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/KismetTextLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "FPSPlayerController.h"
#include "FPSDefaultInterface.h"

// Constructor
AFPSPlayer::AFPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Enable crouch by default
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Add first person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));

	// Add flashlight
	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FirstPersonCamera);
	Flashlight->SetVisibility(false, false);

	// Add skeletal mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerBody"));
	SkeletalMesh->SetupAttachment(FirstPersonCamera);
	SkeletalMesh->SetCastShadow(false);
}


// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Initialize Enhanced Input Subsystem
	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	// Select mapping context
	Subsystem->AddMappingContext(MappingContext, 0);

	// Get Input Component
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind actions
	EIC->BindAction(InputMove, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputMove);
	EIC->BindAction(InputLook, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputLook);
	EIC->BindAction(InputJump, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputJump);
	EIC->BindAction(InputCrouch, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputCrouch);
	EIC->BindAction(InputSprint, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSprint);
	EIC->BindAction(InputToggleFlashlight, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputToggleFlashlight);
	EIC->BindAction(InputFire, ETriggerEvent::Started, this, &AFPSPlayer::OnInputStartFiring);
	EIC->BindAction(InputFire, ETriggerEvent::Completed, this, &AFPSPlayer::OnInputStopFiring);
	EIC->BindAction(InputSelectPrevWeapon, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectPrevWeapon);
	EIC->BindAction(InputSelectNextWeapon, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectNextWeapon);
	EIC->BindAction(InputSwapToLastWeapon, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSwapToLastWeapon);
	EIC->BindAction(InputSelectWeapon1, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectWeaponByNumber, 1);
	EIC->BindAction(InputSelectWeapon2, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectWeaponByNumber, 2);
	EIC->BindAction(InputSelectWeapon3, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectWeaponByNumber, 3);
	EIC->BindAction(InputSelectWeapon4, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectWeaponByNumber, 4);
	EIC->BindAction(InputSelectWeapon5, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectWeaponByNumber, 5);
	EIC->BindAction(InputSelectWeapon5, ETriggerEvent::Triggered, this, &AFPSPlayer::OnInputSelectWeaponByNumber, 6);
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Casts to player controller
	PlayerController = Cast<AFPSPlayerController>(GetController());

	// Save normal walk speed for when sprinting stops
	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Initial camera locations for crouch smoothing
	CameraLocationZStanding = FirstPersonCamera->GetRelativeLocation().Z;
	CameraLocationZCrouching = CameraLocationZStanding + CrouchCameraOffset;

	// Change weapon to initial load, later this will ask the player controller for the value as if loading from a save
	int32 WeaponIndex;
	WeaponFindByNumber(4, WeaponIndex);
	ChangeWeapon(WeaponIndex);
}

// Called when player is despawned
void AFPSPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear all timers
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get player velocity
	PlayerVelocity = GetVelocity().Size();

	// Cancel sprint if player slows down or crouches
	if (bIsSprinting && (PlayerVelocity < 100.f || bIsCrouched))
	{
		// Cancel sprint
		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		bIsSprinting = false;
	}

	// Crouch smoothing per tick
	if (!FMath::IsNearlyEqual(CameraLocationZCurrent, CameraLocationZTarget, 0.000001f))
	{
		CameraLocationZCurrent = FMath::FInterpTo(CameraLocationZCurrent, CameraLocationZTarget, DeltaTime, 10.0f);
		FirstPersonCamera->SetRelativeLocation(FVector(0, 0, CameraLocationZCurrent));
	}

	// Update energy use
	SpendAndRegenEnergy(DeltaTime);
}

// Called from player input of move direction
void AFPSPlayer::OnInputMove_Implementation(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		// Get input into vector
		const FVector2D MoveValue = Value.Get<FVector2D>();
		// GetControlRotation
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Walking forward and back
		if (MoveValue.Y != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			// Add to movement
			AddMovementInput(Direction, MoveValue.Y);
		}

		// Strafe left and right
		if (MoveValue.X != 0.f)
		{
			// Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			// Add to movement
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

// Called from player look input
void AFPSPlayer::OnInputLook_Implementation(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		// Get input into vector
		const FVector2D LookValue = Value.Get<FVector2D>();

		// Looking in X direction
		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}
		
		// Looking in Y direction
		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}

// Called from player jump input
void AFPSPlayer::OnInputJump_Implementation()
{
	// Check if player is crouched
	if (bIsCrouched)
	{
		// If crouched, jump input will remove crouch
		UnCrouch();
	}
	else
	{
		// Make player jump
		Jump();
	}
}

// Called from player crouch input
void AFPSPlayer::OnInputCrouch_Implementation()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

// Called when player starts to crouch
void AFPSPlayer::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	// Current camera position after crouch	
	CameraLocationZCurrent = ScaledHalfHeightAdjust + CameraLocationZStanding;
	// Camera location to smoothly transition to crouch
	CameraLocationZTarget = CameraLocationZCrouching;
}

// Called when player stops crouching
void AFPSPlayer::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	// Current camera position after standing back up
	CameraLocationZCurrent = CameraLocationZStanding - ScaledHalfHeightAdjust;
	// Camera location to smoothly transition to, go back to standing
	CameraLocationZTarget = CameraLocationZStanding;
}

// Called from player sprint input
void AFPSPlayer::OnInputSprint_Implementation()
{
	if (bCanSprint)
	{
		if (bIsSprinting)
		{
			// Return to normal walk speed
			GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		}
		else
		{
			// Sprint
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

			// If crouched, then stand back up
			if (bIsCrouched)
			{
				UnCrouch();
			}
		}
		bIsSprinting = !bIsSprinting;
	}
}

// Called from player flashlight input
void AFPSPlayer::OnInputToggleFlashlight_Implementation()
{
	if (bIsFlashlightOn)
	{
		Flashlight->SetVisibility(false, false);
	}
	else
	{
		Flashlight->SetVisibility(true, false);
	}
	bIsFlashlightOn = !bIsFlashlightOn;
}

// Called when player first presses fire button
void AFPSPlayer::OnInputStartFiring_Implementation()
{
	if (bCanFireWeapon)
	{
		// Prevent repeated input actions from firing weapon faster than intended
		bCanFireWeapon = false;

		// Automatic weapons use a looping timer for continuous fire
		if (PlayerWeapon->WeaponBehavior.bIsAutomatic)
		{
			GetWorld()->GetTimerManager().SetTimer(WeaponFireTimer, this, &AFPSPlayer::WeaponFire, PlayerWeapon->WeaponBehavior.FireRate, true);
		}

		WeaponFire();

		// Wait a delay before before triggering function to allow firing again
		GetWorld()->GetTimerManager().SetTimer(FireDelayTimer, this, &AFPSPlayer::AllowWeaponFire, PlayerWeapon->WeaponBehavior.FireRate, false);
	}
}

// Called when player releases fire button
void AFPSPlayer::OnInputStopFiring_Implementation()
{
	// Stop the automatic firing loop
	GetWorld()->GetTimerManager().ClearTimer(WeaponFireTimer);
}

// Called when the player wants to move to the previous weapon
void AFPSPlayer::OnInputSelectPrevWeapon_Implementation()
{
	ChangeWeapon(CurrentWeaponIndex - 1);
}

// Called when the player wants to move to the next weapon
void AFPSPlayer::OnInputSelectNextWeapon_Implementation()
{
	ChangeWeapon(CurrentWeaponIndex + 1);
}

// Called when the player wants to swap to the last held weapon
void AFPSPlayer::OnInputSwapToLastWeapon_Implementation()
{
	ChangeWeapon(LastWeaponIndex);
}

// Called when player uses one of the direct weapon selection keys
void AFPSPlayer::OnInputSelectWeaponByNumber_Implementation(const int32 InWeaponNumber)
{
	int32 WeaponIndex;
	WeaponFindByNumber(InWeaponNumber, WeaponIndex);
	ChangeWeapon(WeaponIndex);
}

// Called when the game actually wants the weapon to be triggered to fire
void AFPSPlayer::WeaponFire_Implementation()
{
	// Check ammo
	if (AmmoInClip > 0)
	{
		// Tell weapon class to fire and provide the vectors for where the ammo goes to the weapon
		PlayerWeapon->Fire(FirstPersonCamera->GetComponentTransform(), FirstPersonCamera->GetForwardVector(), bIsCrouched);

		// Play montage on the skeletal mesh specified by weapon on the skeleton for arm movement
		SkeletalMesh->GetAnimInstance()->Montage_Play(PlayerWeapon->MeshFiringMontage);

		AmmoInClip--;
		// Update HUD
		IFPSDefaultInterface::Execute_UpdateHUDClipAmmo(PlayerController->InGameHUDWidget, UKismetTextLibrary::Conv_IntToText(AmmoInClip));
	}
	else
	{
		// Trigger reload
	}
}

void AFPSPlayer::ChangeWeapon_Implementation(const int32 InWeaponIndex)
{
	// Only continue if the weapon selected is actually different and the index is valid
	if (!(InWeaponIndex == CurrentWeaponIndex) && WeaponInventory.IsValidIndex(InWeaponIndex))
	{
		// Clear automatic timer if player is firing
		OnInputStopFiring();

		// Stop allowing fire and trigger animation to hide the arms bIsSwappingWeapons is used by the animation state machine
		bCanFireWeapon = false;
		bIsSwappingWeapons = true;

		// Start timer to raise arms back up and allow fire
		GetWorld()->GetTimerManager().SetTimer(WeaponSwapDelayTimer, this, &AFPSPlayer::WeaponSwapEnd, WeaponSwapDelay, false);

		// Update current and last weapon indexes
		LastWeaponIndex = CurrentWeaponIndex;
		CurrentWeaponIndex = InWeaponIndex;
	}
}

// Called at end of delay between swapping weapons
void AFPSPlayer::WeaponSwapEnd()
{
	// Destroy and respawn weapon, this is moved to after the delay as you could see the swapped weapon while the arms were moving down before
	if (IsValid(PlayerWeapon))
	{
		PlayerWeapon->Destroy();
	}
	PlayerWeapon = GetWorld()->SpawnActor<AFPSPlayerWeapon>(WeaponInventory[CurrentWeaponIndex].Get(), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	PlayerWeapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RH_Rifle"));

	bIsSwappingWeapons = false;
	bCanFireWeapon = true;
}

// Called by delay timer to allow firing of weapon again
void AFPSPlayer::AllowWeaponFire()
{
	bCanFireWeapon = true;
	GetWorld()->GetTimerManager().ClearTimer(FireDelayTimer);
}


// Called to find the index of a weapon based on it's keypress number, finds the first in the array with that number
void AFPSPlayer::WeaponFindByNumber_Implementation(const int32 InWeaponNumber, int32& OutWeaponIndex)
{
	OutWeaponIndex = -1;

	for (int i = 0; i < WeaponInventory.Num(); i++)
	{
		AFPSPlayerWeapon* WeaponToCheck = WeaponInventory[i].GetDefaultObject();
		if (WeaponToCheck->WeaponBehavior.WeaponNumber == InWeaponNumber)
		{
			OutWeaponIndex = i;
		}
	}
}

// Called when animation blueprint wants player info for state machine
void AFPSPlayer::GetPlayerAnimState_Implementation(float& OutPlayerVelocity, bool& bOutIsFalling, bool& bOutHasRifle, bool& bOutIsSwappingWeapons)
{
	OutPlayerVelocity = PlayerVelocity;
	bOutIsFalling = GetCharacterMovement()->IsFalling();
	bOutHasRifle = true;
	bOutIsSwappingWeapons = bIsSwappingWeapons;
}

// Called every tick to manage energy use and regen
void AFPSPlayer::SpendAndRegenEnergy(float DeltaTime)
{
	// Flag to update the energy HUD if values change
	bool bUpdateEnergyHUD = false;
	// Flag if any energy was used this tick
	bool bUsedEnergy = false;

	// If player is sprinting, check for conditions to cancel and spend energy
	if (bIsSprinting)
	{
		// If sprint costs energy deduct energy and cancel if out
		if (SprintEnergyCost > 0.f)
		{
			// Flag that energy used this tick
			bUsedEnergy = true;

			// Find new current energy or zero whichever is less
			CurrentEnergy = FMath::Max3((CurrentEnergy - (SprintEnergyCost * DeltaTime)), 0.f, 0.f);
			// Flag to update the HUD
			bUpdateEnergyHUD = true;

			// Cancel if no energy left
			if (CurrentEnergy <= 0.f)
			{
				GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
				bIsSprinting = false;
			}
		}
	}

	// Deduct from energy if flashlight is on and costs energy
	if (bIsFlashlightOn && FlashlightEnergyCost > 0.f)
	{
		// Flag that energy was used this tick
		bUsedEnergy = true;

		// Find new current energy or zero whichever is less
		CurrentEnergy = FMath::Max3((CurrentEnergy - (FlashlightEnergyCost * DeltaTime)), 0.f, 0.f);
		// Flag to update the HUD
		bUpdateEnergyHUD = true;

		// If no energy turn off flashlight
		if (CurrentEnergy <= 0.f)
		{
			Flashlight->SetVisibility(false, false);
			bIsFlashlightOn = false;
		}
	}

	// Regen energy
	if (CurrentEnergy != MaxEnergy)
	{
		// Check if timer is already running and that no energy was used this tick
		if (!GetWorld()->GetTimerManager().IsTimerActive(EnergyRegenHandle) && !bUsedEnergy)
		{
			// Set timer if not
			GetWorld()->GetTimerManager().SetTimer(EnergyRegenHandle, this, &AFPSPlayer::StartEnergyRegen, EnergyRegenDelay, false);
			// Delay of 0 and the timer doesn't activate, so do it manually
			if (EnergyRegenDelay == 0.f)
			{
				bDoRegenEnergy = true;
			}
		}

		// If energy is maxed or energy was used stop regenerating
		if ((CurrentEnergy == MaxEnergy) || bUsedEnergy)
		{
			GetWorld()->GetTimerManager().ClearTimer(EnergyRegenHandle);
			bDoRegenEnergy = false;
		}

		// Check if time to regenerate and do so if ready
		if (bDoRegenEnergy)
		{
			// Find new current energy by adding the regen unless max energy
			CurrentEnergy = FMath::Min3((CurrentEnergy + (EnergyRegenRate * DeltaTime)), MaxEnergy, MaxEnergy);
			bUpdateEnergyHUD = true;
		}
	}

	// Update HUD if energy changed flag was set by any previous functions
	if (bUpdateEnergyHUD)
	{
		IFPSDefaultInterface::Execute_UpdateHUDEnergyPercent(PlayerController->InGameHUDWidget, (CurrentEnergy / 100));
	}
}

// Called by timer when it's time to start regenerating energy
void AFPSPlayer::StartEnergyRegen()
{
	bDoRegenEnergy = true;
}