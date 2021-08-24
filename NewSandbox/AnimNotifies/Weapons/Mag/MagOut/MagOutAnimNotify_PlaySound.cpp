#include "MagOutAnimNotify_PlaySound.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UMagOutAnimNotify_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (!Weapon)
		return;

	else
		UGameplayStatics::SpawnSoundAttached(Weapon->MagOutSound, MeshComp);
}
