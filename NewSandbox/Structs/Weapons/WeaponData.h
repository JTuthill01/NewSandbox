#pragma once

#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxMagAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentTotalAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxTotalAmmo;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 LowAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CriticalHit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Icon)
	class UTexture2D* WeaponIcon;
};