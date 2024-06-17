// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSDefaultInterface.h"
#include "FPSPlayerWeapon.h"
#include "FPSPlayer.generated.h"


UCLASS()
class ESCAPEFROMAREA51_API AFPSPlayer : public ACharacter, public IFPSDefaultInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when player is despawned
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called when player starts to crouch
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	// Called when player stops crouching
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

private:
	// Player controller
	class AFPSPlayerController* PlayerController;

	// Normal walking speed, used in Sprinting to return to normal
	float NormalWalkSpeed;

	// Used for the crouch smoothing action
	float CameraLocationZStanding;
	float CameraLocationZCrouching;
	float CameraLocationZCurrent = 0.f;
	float CameraLocationZTarget;

	// Used as a flag to trigger energy regeneration
	bool bDoRegenEnergy;

	// Timer for energy regeneration
	FTimerHandle EnergyRegenHandle;

	// Called within tick to update energy
	void SpendAndRegenEnergy(float DeltaTime);

	// Called by timer when it's time to start regenerating energy
	void StartEnergyRegen();

	// Timer for weapon fire rate
	FTimerHandle WeaponFireTimer;

	// Timer for delay before allowing fire again
	FTimerHandle FireDelayTimer;

	// Called by timer when the weapon can fire again
	void AllowWeaponFire();

	// Current weapon index
	int32 CurrentWeaponIndex = -1;

	// Previous weapon index
	int32 LastWeaponIndex = -1;

	// Timer for weapon swap
	FTimerHandle WeaponSwapDelayTimer;

	// Called by timer when the weapon swap is complete
	void WeaponSwapEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* OBJECTS */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpotLightComponent* Flashlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Inventory")
		TArray<TSubclassOf<AFPSPlayerWeapon>> WeaponInventory;

	// Player weapon, the actual weapon currently spawned not the array
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	class AFPSPlayerWeapon* PlayerWeapon;


	/* PUBLIC BLUEPRINTABLE VARIABLES */
	// Input mapping context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
		class UInputMappingContext* MappingContext;
		
	// Directional movement input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputMove;

	// Camera movement input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputLook;

	// Jump input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputJump;

	// Crouch input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputCrouch;

	// Sprint input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSprint;

	// Flashlight toggle input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputToggleFlashlight;

	// Weapon fire input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputFire;

	// Input to Select Previous Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectPrevWeapon;

	// Input to Select Next Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectNextWeapon;

	// Input to Swap to Last Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSwapToLastWeapon;

	// Input to Select Weapon #1
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectWeapon1;

	// Input to Select Weapon #2
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectWeapon2;

	// Input to Select Weapon #4
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectWeapon3;

	// Input to Select Weapon #5
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectWeapon4;

	// Input to Select Weapon #5
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectWeapon5;

	// Input to Select Weapon #6
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
		class UInputAction* InputSelectWeapon6;

	// Sprint speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
		float SprintSpeed = 900.f;

	// Can player sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
		bool bCanSprint = true;

	// Camera offset when crouching, this is different from the half height and is part of the crouch smoothing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
		float CrouchCameraOffset = -30.f;

	// Weapon swap delay, the time to wait while the player is in between weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Movement")
		float WeaponSwapDelay = 0.4f;
	
	// Current player energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources");
		float CurrentEnergy = 100.f;

	// Player energy, used for sprinting and anything else desired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		float MaxEnergy = 100.f;

	// Player energy regen rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		float EnergyRegenRate = 20.f;

	// Time in seconds to wait before regen energy starts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		float EnergyRegenDelay = 2.f;

	// Energy cost of sprinting per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		float SprintEnergyCost = 10.f;

	// Energy cost of flashlight per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		float FlashlightEnergyCost = 5.f;

	// Player current ammo in clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		int32 AmmoInClip = 0;

	// Player current ammoo in inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Resources")
		int32 AmmoInInv = 0;

	// Player velocity, updated every tick
	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
		float PlayerVelocity;
	
	// Is player currently sprinting?
	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
		bool bIsSprinting;

	// Is flashlight on?
	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
		bool bIsFlashlightOn;

	// Can the player fire a weapon right now?
	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
		bool bCanFireWeapon;

	// Is the player currently swapping weapons
	UPROPERTY(BlueprintReadOnly, Category = "Player Status")
		bool bIsSwappingWeapons;


	
	/* PUBLIC BLUEPRINTABLE FUNCTIONS */

	// Move Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputMove(const FInputActionValue& Value);
		virtual void OnInputMove_Implementation(const FInputActionValue& Value);

	// Look Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputLook(const FInputActionValue& Value);
		virtual void OnInputLook_Implementation(const FInputActionValue& Value);

	// Jump Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputJump();
		virtual void OnInputJump_Implementation();

	// Crouch Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputCrouch();
		virtual void OnInputCrouch_Implementation();

	// Sprint Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputSprint();
		virtual void OnInputSprint_Implementation();

	// Flashlight Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputToggleFlashlight();
		virtual void OnInputToggleFlashlight_Implementation();

	// Player presses fire button Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputStartFiring();
		virtual void OnInputStartFiring_Implementation();

	// Player release fire button Input Action handler
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputStopFiring();
		virtual void OnInputStopFiring_Implementation();

	// Player selects previous weapon
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputSelectPrevWeapon();
		virtual void OnInputSelectPrevWeapon_Implementation();

	// Player selects next weapon
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputSelectNextWeapon();
		virtual void OnInputSelectNextWeapon_Implementation();

	// Player wants to swap to the last weapon
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputSwapToLastWeapon();
	virtual void OnInputSwapToLastWeapon_Implementation();

	// Player selects a weapon by number
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input Actions")
		void OnInputSelectWeaponByNumber(const int32 InWeaponNumber);
		virtual void OnInputSelectWeaponByNumber_Implementation(const int32 InWeaponNumber);


	// The actual action of firing the weapon, this may be looped by a timer too
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
		void WeaponFire();
		virtual void WeaponFire_Implementation();

	// Change weapon to another by the array index
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
		void ChangeWeapon(const int32 InWeaponIndex);
		virtual void ChangeWeapon_Implementation(const int32 InWeaponIndex);

	// Take weapon name, return current array index
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapons")
		void WeaponFindByNumber(const int32 InWeaponNumber, int32& OutWeaponIndex);
		virtual void WeaponFindByNumber_Implementation(const int32 InWeaponNumber, int32& OutWeaponIndex);

	// Interface implementation IFPSDefaultInterface
		void GetPlayerAnimState(float& OutPlayerVelocity, bool& bOutIsFalling, bool& bOutHasRifle, bool& bOutIsSwappingWeapons);
		virtual void GetPlayerAnimState_Implementation(float& OutPlayerVelocity, bool& bOutIsFalling, bool& bOutHasRifle, bool& bOutIsSwappingWeapons);

};
