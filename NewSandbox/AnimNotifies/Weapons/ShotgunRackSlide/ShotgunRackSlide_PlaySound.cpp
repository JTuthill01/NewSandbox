#include "ShotgunRackSlide_PlaySound.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UShotgunRackSlide_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AWeaponBase* Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (Weapon)
		UGameplayStatics::SpawnSoundAttached(Weapon->ShotgunRackSlideSound, MeshComp);
}
