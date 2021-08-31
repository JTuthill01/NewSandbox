#include "TT33.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ATT33::ATT33() = default;

void ATT33::WeaponFire()
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

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATT33::ResetCanFire, FireTimer, false);
	}
}

void ATT33::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ATT33::ResetIsReloading, ReloadTimer, false);
}

void ATT33::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}

void ATT33::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bCanFire = true;

	bIsReloading = false;
}
