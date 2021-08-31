#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "ShortStrokeAR.generated.h"

UCLASS()
class NEWSANDBOX_API AShortStrokeAR : public AWeaponBase
{
	GENERATED_BODY()

public:
	AShortStrokeAR();
	
protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
	void ResetCanFire();
};
