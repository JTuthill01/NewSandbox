#include "L86.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AL86::AL86() = default;

void AL86::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectTransform.GetRotation() = EjectQuat;

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireTransform.GetRotation() = FireQuat;

	if (WeaponAnimInstance)
	{
		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		FireTimer = WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AL86::ResetCanFire, FireTimer, false);
	}
}

void AL86::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AL86::ResetIsReloading, ReloadTimer, false);
}

void AL86::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}

void AL86::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bCanFire = true;

	bIsReloading = false;
}
