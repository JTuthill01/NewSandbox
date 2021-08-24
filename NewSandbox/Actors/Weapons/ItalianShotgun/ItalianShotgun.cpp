#include "ItalianShotgun.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"

AItalianShotgun::AItalianShotgun() = default;

void AItalianShotgun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AItalianShotgun::ShotgunReload()
{
	Super::ShotgunReload();
}

void AItalianShotgun::WeaponFire()
{
	Super::WeaponFire();

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectTransform.GetRotation() = EjectQuat;

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireTransform.GetRotation() = FireQuat;

	if (WeaponAnimInstance)
	{
		WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());
	}
}

void AItalianShotgun::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	float Length = PlayerRef->Instance->Montage_Play(PlayerRef->ItalianReloadMonatge[EShoutgunReload::ESR_ReloadStart]);

	WeaponAnimInstance->Montage_Play(ItalianReloadMonatge[EShoutgunReload::ESR_ReloadStart]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AItalianShotgun::ShotgunReloadLoop, Length, false);

	bIsReloading = true;
}

void AItalianShotgun::ShotgunReloadLoop()
{
	Super::ShotgunReloadLoop();

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	CurrentAmmo++;

	CurrentTotalAmmo--;

	float Length = PlayerRef->Instance->Montage_Play(PlayerRef->ItalianReloadMonatge[EShoutgunReload::ESR_ReloadLoop]);

	WeaponAnimInstance->Montage_Play(ItalianReloadMonatge[EShoutgunReload::ESR_ReloadLoop]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AItalianShotgun::UpdateReloadAmmo, Length, false);
}

void AItalianShotgun::ShotgunReloadEnd()
{
	Super::ShotgunReloadEnd();

	WeaponAnimInstance->Montage_Play(ItalianReloadMonatge[EShoutgunReload::ESR_ReloadEnd]);

	PlayerRef->Instance->Montage_Play(PlayerRef->ItalianReloadMonatge[EShoutgunReload::ESR_ReloadEnd]);

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	bIsReloading = false;
}

void AItalianShotgun::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	PlayerRef->ShotgunAmmoUpdate.Broadcast(CurrentTotalAmmo, CurrentAmmo);

	if (CurrentAmmo >= MaxMagAmmo)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}

