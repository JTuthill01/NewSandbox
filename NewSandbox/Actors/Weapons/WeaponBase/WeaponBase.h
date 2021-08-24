#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewSandbox/Enums/Enums.h"
#include "NewSandbox/Structs/Weapons/WeaponData.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);

UCLASS()
class NEWSANDBOX_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

public:
	//Getters
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE FName GetSocketName() const { return SocketName; }
	FORCEINLINE EWeaponName GetCurrentWeaponName() const { return WeaponName; }
	FORCEINLINE EWeaponClass GetCurrentWeaponClass() const { return WeaponClass; }
	FORCEINLINE FWeaponData GetWeaponData() const { return WeaponData; }
	FORCEINLINE bool GetCanFire() const { return bCanFire;}
	FORCEINLINE bool GetIsFiring() const { return bIsFiring; }
	FORCEINLINE bool GetCanReload() const { return bCanReload; }
	FORCEINLINE bool GetIsReloading() const { return bIsReloading; }
	FORCEINLINE bool GetIsFirstSlotFull() const { return bIsFirstSlotFull; }
	FORCEINLINE bool GetIsSecondSlotFull() const { return bIsSecondSlotFull; }

	//Setters
	FORCEINLINE void SetIsFirstSlotFull(bool IsFirstSlotFull) { bIsFirstSlotFull = IsFirstSlotFull; }
	FORCEINLINE void SetIsSecondSlotFull(bool IsSecondSlotFull) { bIsSecondSlotFull = IsSecondSlotFull; }
	FORCEINLINE void SetCanFire(bool CanFire) { bCanFire = CanFire; }
	FORCEINLINE void SetCanReload(bool Reload) { bCanReload = Reload; }
	FORCEINLINE void SetIsReloading(bool IsReloading) { bIsReloading = IsReloading; }

public:
	UFUNCTION(BlueprintCallable)
	AWeaponBase* SpawnWeapon(class APlayerCharacter* Player, AWeaponBase* CurrentWeapon, AWeaponBase* Slot1, AWeaponBase* Slot2, TSubclassOf<AWeaponBase> WeaponToSpawn, EWeaponSlot WeaponSlot, EHasWeapon HasWeapon);

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable)
	virtual void WeaponFire();

	UFUNCTION(BlueprintCallable)
	virtual void WeaponReload();

	UFUNCTION(BlueprintCallable)
	virtual void ShotgunReload();

	UFUNCTION(BlueprintCallable)
	virtual void ShotgunReloadStart();

	UFUNCTION(BlueprintCallable)
	virtual void ShotgunReloadLoop();

	UFUNCTION(BlueprintCallable)
	virtual void ShotgunReloadEnd();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* ShotgunRackSlideSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* RackSlideSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* MagOutSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* MagInSound;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged WeaponChanged;

public:
	bool HasFullMag();

	bool IsAmmoFull();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	class APlayerCharacter* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponMesh)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	class UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* WeaponReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	class UNiagaraSystem* AmmoEject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	class UNiagaraSystem* FireFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponData)
	FWeaponData WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enums)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enums)
	EFireType FireType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enums)
	EShotgunShellType CurrentLoadedShell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enums)
	EWeaponClass WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enums)
	int32 CrosshairIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSocketName)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	bool bShouldReload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData)
	bool bIsFiring;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData)
	bool bCanFire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData)
	bool bCanReload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData)
	bool bIsReloading;

	class UAnimInstance* WeaponAnimInstance;

protected:
	int32 ReloadCurrentAmmo;
	int32 CurrentAmmo;
	int32 CurrentTotalAmmo;
	int32 MaxTotalAmmo;
	int32 MaxMagAmmo;

	FQuat EjectQuat;
	FTransform EjectTransform;
	FQuat FireQuat;
	FTransform FireTransform;

	float ReloadTimer;

	FTimerHandle ReloadTimerHandle;

private:
	void BulletTrace(FHitResult& HitResult, FTransform& ProjectileTransform);
	void AddDamage(FHitResult HitResult, bool bDoesImplimentInterface = false);
	//void CreateImpactFX(FHitResult HitResult);

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData, meta = (AllowPrivateAccess = "true"))
	bool bIsFirstSlotFull;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData, meta = (AllowPrivateAccess = "true"))
	bool bIsSecondSlotFull;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData, meta = (AllowPrivateAccess = "true"))
	bool bIsThirdSlotFull;

	FHitResult Hit;
};