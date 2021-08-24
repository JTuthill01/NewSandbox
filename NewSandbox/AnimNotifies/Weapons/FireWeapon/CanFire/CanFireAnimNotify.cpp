#include "CanFireAnimNotify.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UCanFireAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Weapon = Cast<AWeaponBase>(MeshComp->GetOwner());

	if (Weapon)
		Weapon->SetCanFire(true);
}
