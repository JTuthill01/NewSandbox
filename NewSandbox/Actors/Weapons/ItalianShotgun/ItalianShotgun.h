#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "ItalianShotgun.generated.h"

UCLASS()
class NEWSANDBOX_API AItalianShotgun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AItalianShotgun();

	virtual void WeaponFire() override;

protected:
	virtual void BeginPlay() override;

	virtual void ShotgunReload() override;

	virtual void ShotgunReloadStart() override;

	virtual void ShotgunReloadLoop() override;

	virtual void ShotgunReloadEnd() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> ItalianReloadMonatge;

private:
	void UpdateReloadAmmo();

	FTimerHandle ShotgunReloadTimerHandle;
};
