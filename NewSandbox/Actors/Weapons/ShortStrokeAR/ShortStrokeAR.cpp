#include "ShortStrokeAR.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AShortStrokeAR::AShortStrokeAR() = default;

void AShortStrokeAR::WeaponFire()
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

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AShortStrokeAR::ResetCanFire, FireTimer, false);
	}
}

void AShortStrokeAR::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AShortStrokeAR::ResetIsReloading, ReloadTimer, false);
}

void AShortStrokeAR::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}

void AShortStrokeAR::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bCanFire = true;

	bIsReloading = false;
}
