#include "SKS.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ASKS::ASKS() = default;

void ASKS::WeaponFire()
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

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ASKS::ResetCanFire, FireTimer, false);
	}
}

void ASKS::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ASKS::ResetIsReloading, ReloadTimer, false);
}

void ASKS::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}

void ASKS::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bCanFire = true;

	bIsReloading = false;
}
