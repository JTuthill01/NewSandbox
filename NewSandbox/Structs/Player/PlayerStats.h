#pragma once

#include "PlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 CurrentHealth;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 MaxHealth; 

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 CurrentArmor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 MaxArmor;
};