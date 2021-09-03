#include "ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystem.h"
#include "NewSandbox/ImpactMaterials/ImpactPhysicalMaterial.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectileBase::AProjectileBase() : bIsCausingRadiusDamage(false), ProjectileLifeTime(5.F)
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 10'000.F;
	ProjectileMovement->MaxSpeed = 10'000.F;
	ProjectileMovement->ProjectileGravityScale = 0.2F;
	ProjectileMovement->bSweepCollision = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(8.F);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->bReturnMaterialOnMove = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetupAttachment(Sphere);

	SetRootComponent(Sphere);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerRef = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AProjectileBase::ProjectileStop);
}

void AProjectileBase::ProjectileStop(const FHitResult& HitResult)
{
	FTransform HitTransform = ProjectileMesh->GetComponentTransform();

	if (bIsCausingRadiusDamage)
	{
		Destroy();

		/*ExplodeOnImpact(HitResult); */

		SpawnExplosiveImpactFX(HitResult);
	}

	else
	{
		Destroy();

		SpawnRegularImpactFX(HitResult);
	}
}

void AProjectileBase::ExplodeOnImpact(const FHitResult& HitResult)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	ActorsToIgnore.Add(PlayerRef->GetCurrentWeapon());
	ActorsToIgnore.Add(this);

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), HitResult.ImpactPoint, PlayerRef->GetCurrentWeapon()->GetWeaponData().DamageRadius, TraceObjects, NULL, ActorsToIgnore, OutActors);

	uint8 Len = OutActors.Num();

	for (uint8 i = 0; i < Len; ++i)
	{
		OutActors[i] = HitResult.Actor.Get();

		if (OutActors[i])
		{
				
		}
    }
}

void AProjectileBase::SpawnExplosiveImpactFX(FHitResult HitResult)
{
	FVector TempVector = ProjectileMesh->GetComponentLocation();

	FRotator TempRotator = ProjectileMesh->GetComponentRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFX, TempVector, TempRotator);
}

void AProjectileBase::SpawnRegularImpactFX(FHitResult HitResult)
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

				UGameplayStatics::SpawnDecalAttached(ImpactDecal, DecalSize, HitComponent, NAME_None, HitResult.Location, Rotation, EAttachLocation::KeepWorldPosition, DecalLifetime);
			}
		}
	}
}
