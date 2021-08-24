#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "NavySMG.generated.h"

UCLASS()
class NEWSANDBOX_API ANavySMG : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ANavySMG();

protected:
	virtual void WeaponReload() override;

	virtual void WeaponFire() override;

private:
	void ResetIsReloading();
};
