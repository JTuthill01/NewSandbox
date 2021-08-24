#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "SKS.generated.h"

UCLASS()
class NEWSANDBOX_API ASKS : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ASKS();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
};
