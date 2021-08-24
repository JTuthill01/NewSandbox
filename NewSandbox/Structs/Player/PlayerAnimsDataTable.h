#pragma once

#include "Engine/DataTable.h"
#include "PlayerAnimsDataTable.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAnimsDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAnimSequence* IdleSequence;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAnimSequence* WalkSequence;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAnimSequence* SprintSequence;
};