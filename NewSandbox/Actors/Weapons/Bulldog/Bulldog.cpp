#include "Bulldog.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"

ABulldog::ABulldog() = default;

void ABulldog::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void ABulldog::ShotgunReload()
{
	Super::ShotgunReload();
}

void ABulldog::WeaponFire()
{
	Super::WeaponFire();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireTransform.GetRotation() = FireQuat;

	if (WeaponAnimInstance)
	{
		WeaponAnimInstance->Montage_Play(WeaponFireMontage);

		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());
	}
}

void ABulldog::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	float Length = PlayerRef->Instance->Montage_Play(PlayerRef->BulldogReloadMonatge[EShoutgunReload::ESR_ReloadStart]);

	WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[EShoutgunReload::ESR_ReloadStart]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &ABulldog::ShotgunReloadLoop, Length, false);
}

void ABulldog::ShotgunReloadLoop()
{
	Super::ShotgunReloadLoop();

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	CurrentAmmo++;

	CurrentTotalAmmo--;

	float Length = PlayerRef->Instance->Montage_Play(PlayerRef->BulldogReloadMonatge[EShoutgunReload::ESR_ReloadLoop]);

	WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[EShoutgunReload::ESR_ReloadLoop]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &ABulldog::UpdateReloadAmmo, Length, false);
}

void ABulldog::ShotgunReloadEnd()
{
	Super::ShotgunReloadEnd();

	WeaponAnimInstance->Montage_Play(BulldogReloadMonatge[EShoutgunReload::ESR_ReloadEnd]);

	PlayerRef->Instance->Montage_Play(PlayerRef->BulldogReloadMonatge[EShoutgunReload::ESR_ReloadEnd]);

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);
}

void ABulldog::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	PlayerRef->ShotgunAmmoUpdate.Broadcast(CurrentTotalAmmo, CurrentAmmo);

	if (CurrentAmmo >= MaxMagAmmo)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}