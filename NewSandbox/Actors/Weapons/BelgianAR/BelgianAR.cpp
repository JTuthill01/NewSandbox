#include "BelgianAR.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ABelgianAR::ABelgianAR() = default;

void ABelgianAR::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectTransform.GetRotation() = EjectQuat;

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireTransform.GetRotation() = FireQuat;

	if (WeaponAnimInstance)
	{
		FireTimer = WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ABelgianAR::ResetCanFire, FireTimer, false);
	}
}

void ABelgianAR::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ABelgianAR::ResetIsReloading, ReloadTimer, false);
}

void ABelgianAR::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}

void ABelgianAR::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}
