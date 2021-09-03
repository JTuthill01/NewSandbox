#include "AmericanShotgun.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"

AAmericanShotgun::AAmericanShotgun() = default;

void AAmericanShotgun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AAmericanShotgun::ShotgunReload()
{
	Super::ShotgunReload();
}

void AAmericanShotgun::WeaponFire()
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

		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAmericanShotgun::ResetCanFire, FireTimer, false);
	}
}

void AAmericanShotgun::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	float Length = PlayerRef->Instance->Montage_Play(PlayerRef->AmericanShotgunReloadMonatge[EShoutgunReload::ESR_ReloadStart]);

	WeaponAnimInstance->Montage_Play(AmericanReloadMonatge[EShoutgunReload::ESR_ReloadStart]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AAmericanShotgun::ShotgunReloadLoop, Length, false);
}

void AAmericanShotgun::ShotgunReloadLoop()
{
	Super::ShotgunReloadLoop();

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	CurrentAmmo++;

	CurrentTotalAmmo--;

	float Length = PlayerRef->Instance->Montage_Play(PlayerRef->AmericanShotgunReloadMonatge[EShoutgunReload::ESR_ReloadLoop]);

	WeaponAnimInstance->Montage_Play(AmericanReloadMonatge[EShoutgunReload::ESR_ReloadLoop]);

	GetWorldTimerManager().SetTimer(ShotgunReloadTimerHandle, this, &AAmericanShotgun::UpdateReloadAmmo, Length, false);
}

void AAmericanShotgun::ShotgunReloadEnd()
{
	Super::ShotgunReloadEnd();

	FireTimer = WeaponAnimInstance->Montage_Play(AmericanReloadMonatge[EShoutgunReload::ESR_ReloadEnd]);

	PlayerRef->Instance->Montage_Play(PlayerRef->AmericanShotgunReloadMonatge[EShoutgunReload::ESR_ReloadEnd]);

	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	bCanFire = true;
}

void AAmericanShotgun::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ShotgunReloadTimerHandle);

	PlayerRef->ShotgunAmmoUpdate.Broadcast(CurrentTotalAmmo, CurrentAmmo);

	if (CurrentAmmo >= MaxMagAmmo)
		ShotgunReloadEnd();
	
	else
		ShotgunReloadLoop();
}

void AAmericanShotgun::ResetCanFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	bCanFire = true;

	bIsReloading = false;
}
