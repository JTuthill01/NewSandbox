#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "M4A1.generated.h"

UCLASS()
class NEWSANDBOX_API AM4A1 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AM4A1();

protected:
	virtual void WeaponReload() override;

	virtual void WeaponFire() override;

private:
	void ResetIsReloading();
};
