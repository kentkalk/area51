#pragma once
// Custom Data Types defined here

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Internationalization/Text.h"
#include "CustomDataTypes.generated.h"

// Defines characteristics of different weapon types
USTRUCT(BlueprintType)
struct FWeaponBehavior
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "WeaponNumber", MakeStructureDefaultValue = "0"))
		int32 WeaponNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsAutomatic", MakeStructureDefaultValue = "False"))
		bool bIsAutomatic;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FireRate", MakeStructureDefaultValue = "0.000000"))
		float FireRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "PelletCount", MakeStructureDefaultValue = "0"))
		int32 PelletCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Range", MakeStructureDefaultValue = "0"))
		int32 Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Spread", MakeStructureDefaultValue = "0"))
		float Spread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "SpreadIfCrouched", MakeStructureDefaultValue = "0"))
		float SpreadIfCrouched;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AmmoType", MakeStructureDefaultValue = "0"))
		int32 MaxAmmoInInventory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "MaxAmmoInClip", MakeStructureDefaultValue = "0"))
		int32 MaxAmmoInClip;
};