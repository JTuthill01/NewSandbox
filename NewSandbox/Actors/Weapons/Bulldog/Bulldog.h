#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Bulldog.generated.h"

UCLASS()
class NEWSANDBOX_API ABulldog : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ABulldog();

	virtual void WeaponFire() override;

protected:

	virtual void BeginPlay() override;

	virtual void ShotgunReload() override;

	virtual void ShotgunReloadStart() override;

	virtual void ShotgunReloadLoop() override;

	virtual void ShotgunReloadEnd() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<class UAnimMontage*> BulldogReloadMonatge;

private:
	void UpdateReloadAmmo();

	FTimerHandle ShotgunReloadLoopTimerHandle;
	FTimerHandle ShotgunReloadTimerHandle;
};
