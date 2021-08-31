#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "AK47.generated.h"

UCLASS()
class NEWSANDBOX_API AAK47 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AAK47();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();

	void ResetCanFire();
};
