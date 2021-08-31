#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "SVD.generated.h"

UCLASS()
class NEWSANDBOX_API ASVD : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ASVD();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
	void ResetCanFire();
};
