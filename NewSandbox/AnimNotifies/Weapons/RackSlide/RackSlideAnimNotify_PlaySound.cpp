#include "RackSlideAnimNotify_PlaySound.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void URackSlideAnimNotify_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (!Weapon)
		return;

	if (Weapon)
		UGameplayStatics::SpawnSoundAttached(Weapon->RackSlideSound, MeshComp);
}
