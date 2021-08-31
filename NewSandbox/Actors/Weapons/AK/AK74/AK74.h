#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "AK74.generated.h"

UCLASS()
class NEWSANDBOX_API AAK74 : public AWeaponBase
{
	GENERATED_BODY()

public:
	AAK74();
	
protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();

	void ResetCanFire();
};
