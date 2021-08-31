#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "BelgianAR.generated.h"

UCLASS()
class NEWSANDBOX_API ABelgianAR : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ABelgianAR();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();

	void ResetCanFire();
};
