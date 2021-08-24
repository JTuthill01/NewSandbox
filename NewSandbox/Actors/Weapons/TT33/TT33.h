#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "TT33.generated.h"

UCLASS()
class NEWSANDBOX_API ATT33 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ATT33();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
};
