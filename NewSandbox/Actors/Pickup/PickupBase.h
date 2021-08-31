#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewSandbox/Interfaces/Interact/Interact.h"
#include "NewSandbox/Enums/PickupEnums.h"
#include "PickupBase.generated.h"

UCLASS()
class NEWSANDBOX_API APickupBase : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

public:
	void InteractableFound_Implementation() override;

	void Interaction_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupType)
	EPickupType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupType)
	EPickupHealthType HealthType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupType)
	EPickupAmmoType AmmoType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Name)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Name)
	FName SwapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Name)
	FName CurrentName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Icon)
	class UTexture2D* IconTexture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Icon)
	class UTexture2D* SwapIconTexture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Icon)
	class UTexture2D* CurrentIconTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spawn)
	TSubclassOf<class AWeaponBase> WeaponToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupProperties)
	int32 HealthPackValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupProperties)
	int32 PistolPickupValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupProperties)
	int32 RiflePickupValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupProperties)
	int32 ShotgunPickupValue;

	UPROPERTY()
	class APlayerCharacter* Player;

private:
	void Spawn();
	void SwapWeaponSpawn();

	bool SetHealth();
	bool SetAmmo();
};
