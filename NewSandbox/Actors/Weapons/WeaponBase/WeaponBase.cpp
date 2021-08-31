#include "WeaponBase.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Interfaces/TakeDamage/TakeDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AWeaponBase::AWeaponBase() : SocketName(TEXT("WeaponSocket")), bCanFire(true), bCanReload(true), bIsReloading(false)
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

	BulletTrace(ImpactResult, ImpactTransform);

	if (ImpactResult.bBlockingHit)
	{
		AddDamage(ImpactResult);

		//CreateImpactFX(ImpactResult);
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

	if (bHasBeenHit)
	{
		FVector Translation = HitResult.Location;
		FVector SocketLocation = WeaponMesh->GetSocketLocation(SocketName);

		FRotator TempRotator = UKismetMathLibrary::FindLookAtRotation(SocketLocation, Translation);

		ProjectileTransform = UKismetMathLibrary::MakeTransform(Translation, TempRotator, Scale);
	}
}

void AWeaponBase::AddDamage(FHitResult HitResult, bool bDoesImplimentInterface)
{
	if (bDoesImplimentInterface)
	{
		AActor* TempActor = HitResult.GetActor();

		if (TempActor)
		{
			ITakeDamageInterface* iTemp = Cast<ITakeDamageInterface>(TempActor);

			if (iTemp)
			{
				if (TempActor->GetClass()->ImplementsInterface(UTakeDamageInterface::StaticClass()))
					iTemp->Execute_TakeWeaponDamage(TempActor, WeaponData, 5.F, HitResult);
			}
		}
	}
}

//void AWeaponBase::CreateImpactFX(FHitResult HitResult)
//{
//	if (UImpactFX_PhysicalMaterial* PhysMat = Cast<UImpactFX_PhysicalMaterial>(HitResult.PhysMaterial))
//	{
//		if (USoundBase* ImpactSound = PhysMat->LineTraceImpactEffect.ImpactSound)
//		{
//			FVector Location = HitResult.Location;
//
//			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
//		}
//
//		if (UNiagaraSystem* HitFX = Cast<UNiagaraSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
//		{
//			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
//			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX, HitResult.Location, Rotation);
//		}
//
//		else if (UParticleSystem* ParticleFX = Cast<UParticleSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
//		{
//			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
//			FVector Location = HitResult.Location;
//
//			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX, Location, Rotation);
//		}
//
//		if (UMaterialInstance* ImpactDecal = PhysMat->LineTraceImpactEffect.ImpactDecal)
//		{
//			if (USceneComponent* HitComponent = HitResult.GetComponent())
//			{
//				FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
//				Rotation.Pitch += 180.0f;
//				FVector DecalSize = PhysMat->LineTraceImpactEffect.DecalSize;
//				float DecalLifetime = PhysMat->LineTraceImpactEffect.DecalLifeTime;
//
//				UGameplayStatics::SpawnDecalAttached(ImpactDecal, DecalSize, HitComponent, NAME_None,
//					HitResult.Location, Rotation, EAttachLocation::KeepWorldPosition, DecalLifetime);
//			}
//		}
//	}
//}

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
