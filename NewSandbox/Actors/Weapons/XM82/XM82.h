#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "XM82.generated.h"

UCLASS()
class NEWSANDBOX_API AXM82 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AXM82();

protected:
	virtual void WeaponFire() override;

	virtual void WeaponReload() override;

private:
	void ResetIsReloading();
};
