#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class NEWSANDBOX_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

public:
	FORCEINLINE float GetProjectileLifeSpan() { return ProjectileLifeTime; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void ProjectileStop(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	virtual void ExplodeOnImpact(const FHitResult& HitResult);

	UFUNCTION()
	virtual void SpawnExplosiveImpactFX(FHitResult HitResult);

	UFUNCTION()
	virtual void SpawnRegularImpactFX(FHitResult HitResult);

protected:
	UPROPERTY()
	class APlayerCharacter* PlayerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Collision)
	class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	class UParticleSystemComponent* TrailFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	class UNiagaraSystem* ExplosionFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Damage)
	bool bIsCausingRadiusDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = LifeSpan)
	float ProjectileLifeTime;
};
