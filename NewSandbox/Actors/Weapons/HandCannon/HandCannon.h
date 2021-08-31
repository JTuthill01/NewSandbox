#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "HandCannon.generated.h"

UCLASS()
class NEWSANDBOX_API AHandCannon : public AWeaponBase
{
	GENERATED_BODY()
public:
	AHandCannon();

protected:
	virtual void WeaponReload() override;

	virtual void WeaponFire() override;

private:
	void ResetIsReloading();
	void ResetCanFire();
};
