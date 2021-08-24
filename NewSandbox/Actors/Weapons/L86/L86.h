#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "L86.generated.h"

UCLASS()
class NEWSANDBOX_API AL86 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AL86();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
};
