#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NewSandbox/Structs/ImpactFX/ImpactEffects.h"
#include "ImpactPhysicalMaterial.generated.h"

UCLASS()
class NEWSANDBOX_API UImpactPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UImpactPhysicalMaterial();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FImpactEffects LineTraceImpactEffect;
};
