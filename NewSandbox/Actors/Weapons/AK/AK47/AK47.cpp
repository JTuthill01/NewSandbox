#include "AK47.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AAK47::AAK47() = default;

void AAK47::WeaponFire()
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

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAK47::ResetCanFire, FireTimer, false);
	}
}

void AAK47::WeaponReload()
{
	Super::WeaponReload();

	if (WeaponAnimInstance)
		ReloadTimer = WeaponAnimInstance->Montage_Play(WeaponReloadMontage);

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AAK47::ResetIsReloading, ReloadTimer, false);
}

void AAK47::ResetIsReloading()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	bIsReloading = false;

	bCanFire = true; 
}

void AAK47::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bIsReloading = false;

	bCanFire = true;
}
