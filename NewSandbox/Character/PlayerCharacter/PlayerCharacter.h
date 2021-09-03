#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NewSandbox/Enums/Enums.h"
#include "NewSandbox/Structs/Player/PlayerAnimsDataTable.h"
#include "NewSandbox/Structs/Player/PlayerStats.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentAmmo, int32, AmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentHealth, int32, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentAnimation, EWeaponName, CurrentName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReloadShotgun, int32, Ammo, int32, TubeCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateCurrentAmmoReload, int32, AmmoCount, int32, MagCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUpdateCurrentWeapon, class UTexture2D*, NewWeaponIcon, FName, NewWeaponName, int32, CrosshairIndex);

UCLASS()
class NEWSANDBOX_API APlayerCharacter : public ACharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	class UAnimInstance* Instance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;;

	virtual APlayerCharacter* GetPlayerRef_Implementation() override;

public:
	//Getters
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE USkeletalMeshComponent* GetPlayerArms() const { return Arms; }
	FORCEINLINE AWeaponBase* GetCurrentWeapon() { return CurrentWeapon; }
	FORCEINLINE AWeaponBase* GetWeaponSlot_01() { return WeaponSlot_01; }
	FORCEINLINE AWeaponBase* GetWeaponSlot_02() { return WeaponSlot_02; }

	FORCEINLINE void SetCurrenWeapon(AWeaponBase* Weapon = nullptr) { CurrentWeapon = Weapon; }

public:

	UFUNCTION(BlueprintCallable)
	bool SpawnWeapon(TSubclassOf<class AWeaponBase> WeaponToSpawn);

	UFUNCTION(BlueprintCallable)
	bool SwapWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintCallable)
	void ShowWeapon();

	UFUNCTION(BlueprintCallable)
	void SetHealth(int32 Amount);

public:
	bool CanSwitchWeapons();

	bool HasOpenSlot();

	bool HasFullHealth();

public:
	UPROPERTY(BlueprintAssignable)
	FReloadShotgun ShotgunAmmoUpdate;

	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentAnimation AnimationName;

	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentWeapon NewWeaponUpdate;

	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentHealth NewHealthAmount;

	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentAmmo CurrentAmmoCount;

	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentAmmoReload AmmoCountReload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animations)
	FPlayerAnimsDataTable PlayerAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunAnimations)
	TArray<UAnimMontage*> AmericanShotgunReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunAnimations)
	TArray<UAnimMontage*> BulldogReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunAnimations)
	TArray<UAnimMontage*> ItalianReloadMonatge;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

#pragma region INPUT
	//Input Actions//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* WeaponFireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* WeaponReloadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* EquipAction;

	//Mapping Contexts//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	int32 BaseMappingPriority = 0;

	//Input Functions//
	virtual void Jump() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchPressed();
	void FirePressed();
	void FireReleased();
	void Interact();
	void Reload();

#pragma endregion

private:
	void InterpCapsuleHalfHeight(float DeltaTime);
	void ScanForPickups();
	void Initialize();
	void Fire();
	void StopFire();
	void FireAnimation();
	void ReloadAnimation();

	bool IsArmorFull();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class AWeaponBase* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* WeaponSlot_01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* WeaponSlot_02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> FireMonatge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> ReloadMonatge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	EHasWeapon HasWeaponEnum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int32 MaxHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int32 MaxArmor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int32 CurrentArmor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WeaponData, meta = (AllowPrivateAccess = "true"))
	bool bIsChangingWeapon;

	bool bIsFirstSlotFull;
	bool bIsSecondSlotFull;

	float CrouchMovementSpeed;
	float CrouchGroundFriction;
	float BaseMovementSpeed;
	float BaseGroundFriction;
	float CrouchingCapsuleHalfHeight;
	float StandingCapsuleHalfHeight;
	float PickupTimer;

	EWeaponSlot WeaponSlotEnum;

	FTimerHandle PickupTimerHandle;
};
