#include "CannotFireAnimNotify.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"


void UCannotFireAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (Weapon)
		Weapon->SetCanFire(false);
}
