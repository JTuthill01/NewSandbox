#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "GermanSMG.generated.h"

UCLASS()
class NEWSANDBOX_API AGermanSMG : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AGermanSMG();

protected:
	virtual void WeaponReload() override;

	virtual void WeaponFire() override;

private:
	void ResetIsReloading();
};
