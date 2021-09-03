#include "WeaponBase.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Interfaces/TakeDamage/TakeDamageInterface.h"
#include "NewSandbox/ImpactMaterials/ImpactPhysicalMaterial.h"
#include "NewSandbox/Actors/Projectiles/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AWeaponBase::AWeaponBase() : CrosshairIndex(1), SocketName(TEXT("WeaponSocket")), bCanFire(true), bCanReload(true), bIsReloading(false)
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCastShadow(false);
	WeaponMesh->bReturnMaterialOnMove = true;

	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	WeaponAnimInstance = WeaponMesh->GetAnimInstance();
}

void AWeaponBase::WeaponFire()
{
	WeaponData.CurrentAmmo--;

	bCanReload = false;

	bCanFire = false;

	FTransform ImpactTransform;
	FHitResult ImpactResult;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (FireType)
	{
	case EFireType::EFT_Hitscan:

		BulletTrace(ImpactResult, ImpactTransform);

		if (ImpactResult.bBlockingHit)
		{
			AddDamage(ImpactResult);

			CreateImpactFX(ImpactResult);
		}

		break;

	case EFireType::EFT_Projectile:

		BulletTrace(ImpactResult, ImpactTransform);

		Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileToSpawn, ImpactTransform, SpawnInfo);

		if (Projectile)
		{
			Projectile->SetLifeSpan(Projectile->GetProjectileLifeSpan());

			if (ImpactResult.bBlockingHit)
				AddDamage(ImpactResult);
		}
		
		break;

	default:
		break;
	}
}

void AWeaponBase::BulletTrace(FHitResult& HitResult, FTransform& ProjectileTransform)
{
	FVector Start{ PlayerRef->GetCamera()->GetComponentLocation() };
	FVector Rotation{ PlayerRef->GetCamera()->GetComponentRotation().Vector() };
	FVector End{ Start + Rotation * 10'000.F };
	FVector Scale = FVector(1.F);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(UGameplayStatics::GetPlayerController(PlayerRef, 0));
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bHasBeenHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	FVector Translation = HitResult.Location;
	FVector SocketLocation = WeaponMesh->GetSocketLocation(FName("Fire_FX_Slot"));

	if (bHasBeenHit)
	{
		FRotator TempRotator = UKismetMathLibrary::FindLookAtRotation(SocketLocation, Translation);

		ProjectileTransform = UKismetMathLibrary::MakeTransform(Translation, TempRotator, Scale);
	}

	else
	{
		FRotator TempRotator = PlayerRef->GetController()->GetControlRotation();

		if (TempRotator == FRotator(0.F))
			return;

		else
			ProjectileTransform = UKismetMathLibrary::MakeTransform(SocketLocation, TempRotator, Scale);
	}
}

void AWeaponBase::AddDamage(FHitResult HitResult)
{
	if (HitResult.bBlockingHit)
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UTakeDamageInterface::StaticClass()))
			ITakeDamageInterface::Execute_TakeWeaponDamage(HitResult.GetActor(), WeaponData, 5.F, HitResult);

		else 
			return;
}

void AWeaponBase::CreateImpactFX(FHitResult HitResult)
{
	if (UImpactPhysicalMaterial* PhysMat = Cast<UImpactPhysicalMaterial>(HitResult.PhysMaterial))
	{
		if (USoundBase* ImpactSound = PhysMat->LineTraceImpactEffect.ImpactSound)
		{
			FVector Location = HitResult.Location;

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
		}

		if (UNiagaraSystem* HitFX = Cast<UNiagaraSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX, HitResult.Location, Rotation);
		}

		else if (UParticleSystem* ParticleFX = Cast<UParticleSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			FVector Location = HitResult.Location;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX, Location, Rotation);
		}

		if (UMaterialInstance* ImpactDecal = PhysMat->LineTraceImpactEffect.ImpactDecal)
		{
			if (USceneComponent* HitComponent = HitResult.GetComponent())
			{
				FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);

				Rotation.Pitch += 180.0f;

				FVector DecalSize = PhysMat->LineTraceImpactEffect.DecalSize;

				float DecalLifetime = PhysMat->LineTraceImpactEffect.DecalLifeTime;

				UGameplayStatics::SpawnDecalAttached(ImpactDecal, DecalSize, HitComponent, NAME_None,
					HitResult.Location, Rotation, EAttachLocation::KeepWorldPosition, DecalLifetime);
			}
		}
	}
}

void AWeaponBase::WeaponReload()
{
	WeaponData.CurrentTotalAmmo -= WeaponData.MaxMagAmmo - WeaponData.CurrentAmmo;

	WeaponData.CurrentAmmo = UKismetMathLibrary::Min(WeaponData.MaxMagAmmo, WeaponData.CurrentTotalAmmo);

	WeaponData.CurrentAmmo = FMath::Clamp(WeaponData.CurrentAmmo, 0, WeaponData.MaxTotalAmmo);

	WeaponData.CurrentTotalAmmo = FMath::Clamp(WeaponData.CurrentTotalAmmo, 0, WeaponData.MaxTotalAmmo);

	bShouldReload = false;
}

void AWeaponBase::ShotgunReload()
{
	ShotgunReloadStart();

	WeaponData.CurrentAmmo = FMath::Clamp(WeaponData.CurrentAmmo, 0, WeaponData.MaxTotalAmmo);

	WeaponData.CurrentTotalAmmo = FMath::Clamp(WeaponData.CurrentTotalAmmo, 0, WeaponData.MaxTotalAmmo);

	bShouldReload = false;
}

void AWeaponBase::SetCurrentTotalAmmo(int32 Amount)
{
	WeaponData.CurrentTotalAmmo += Amount;

	if (WeaponData.CurrentTotalAmmo > WeaponData.MaxTotalAmmo)
		WeaponData.CurrentTotalAmmo = WeaponData.MaxTotalAmmo;

	NewTotalAmmo.Broadcast(WeaponData.CurrentTotalAmmo);
}

bool AWeaponBase::HasFullMag() { return WeaponData.CurrentAmmo >= WeaponData.MaxMagAmmo; }

bool AWeaponBase::IsAmmoFull() { return WeaponData.CurrentTotalAmmo >= WeaponData.MaxTotalAmmo; }

void AWeaponBase::StopFire() { bCanReload = true; }

void AWeaponBase::ShotgunReloadStart() {}

void AWeaponBase::ShotgunReloadLoop() {}

void AWeaponBase::ShotgunReloadEnd() {}
