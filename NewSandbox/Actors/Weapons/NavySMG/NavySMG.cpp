#include "NavySMG.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ANavySMG::ANavySMG() = default;

void ANavySMG::WeaponFire()
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

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ANavySMG::ResetCanFire, FireTimer, false);
	}
}

void ANavySMG::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ANavySMG::ResetIsReloading, ReloadTimer, false);
}

void ANavySMG::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}

void ANavySMG::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bCanFire = true;

	bIsReloading = false;
}
