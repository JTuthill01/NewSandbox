#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NewSandbox/Interfaces/Interact/Interact.h"

APlayerCharacter::APlayerCharacter() : HasWeaponEnum(EHasWeapon::EHW_NoWeapon), MaxHealth(100), CurrentHealth(MaxHealth), MaxArmor(100), CurrentArmor(MaxArmor),bIsCrouching(false), 
	bIsChangingWeapon(false), CrouchMovementSpeed(300.F), CrouchGroundFriction(100.F),  BaseMovementSpeed(650.F), BaseGroundFriction(2.F), CrouchingCapsuleHalfHeight(44.F), 
	StandingCapsuleHalfHeight(88.F), PickupTimer(0.25F)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->bUsePawnControlRotation = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(Camera);
	Arms->SetCastShadow(false);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpCapsuleHalfHeight(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Make sure that we are using a UEnhancedInputComponent; if not, the project is not configured correctly.

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		if (WeaponFireAction)
		{
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Started, this, &APlayerCharacter::FirePressed);
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Completed, this, &APlayerCharacter::FireReleased);
		}

		if (CrouchAction)
			PlayerEnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::CrouchPressed);

		if (MovementAction)
			PlayerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		if (LookAction)
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		if (InteractAction)
			PlayerEnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);

		if (WeaponReloadAction)
			PlayerEnhancedInputComponent->BindAction(WeaponReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);

		if (EquipAction)
			PlayerEnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &APlayerCharacter::EquipWeapon);
	}
}

void APlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	// Make sure that we have a valid PlayerController.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();
			// Add each mapping context, along with their priority values. Higher values out prioritize lower values.
			Subsystem->AddMappingContext(BaseMappingContext, BaseMappingPriority);
		}
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void APlayerCharacter::Initialize()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	Instance = Arms->GetAnimInstance();

	GetWorldTimerManager().SetTimer(PickupTimerHandle, this, &APlayerCharacter::ScanForPickups, PickupTimer, true);

	if (CurrentWeapon)
		AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());
}

void APlayerCharacter::Jump()
{
	if (bIsCrouching)
	{
		bIsCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}

	else
		ACharacter::Jump();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.F)
	{
		AddMovementInput(GetActorForwardVector(), Value[1]);
		AddMovementInput(GetActorRightVector(), Value[0]);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value[1] * -1);
	AddControllerYawInput(Value[0]);
}

void APlayerCharacter::CrouchPressed()
{
	if (!GetCharacterMovement()->IsFalling())
		bIsCrouching = !bIsCrouching;

	if (bIsCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
		GetCharacterMovement()->GroundFriction = CrouchGroundFriction;
	}

	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
		GetCharacterMovement()->GroundFriction = BaseGroundFriction;
	}
}

void APlayerCharacter::FirePressed()
{
	switch (HasWeaponEnum)
	{
	case EHasWeapon::EHW_NoWeapon:
		break;

	case EHasWeapon::EHW_HasWeapon:

		Fire();

		break;

	default:
		break;
	}
}

void APlayerCharacter::FireReleased()
{
	switch (HasWeaponEnum)
	{
	case EHasWeapon::EHW_NoWeapon:
		break;

	case EHasWeapon::EHW_HasWeapon:

		StopFire();

		break;

	default:
		break;
	}
}

void APlayerCharacter::Fire()
{
	if (CurrentWeapon->GetCanFire() && CurrentWeapon->GetWeaponData().CurrentAmmo > 0 && !bIsChangingWeapon && !CurrentWeapon->GetIsReloading())
	{
		CurrentWeapon->WeaponFire();

		FireAnimation();
	}
}

void APlayerCharacter::FireAnimation()
{
	int32 WeaponIndex = static_cast<int32> (CurrentWeapon->GetCurrentWeaponName());

	if (Instance)
	{
		if (FireMonatge.IsValidIndex(WeaponIndex))
		{
			Instance->Montage_Play(FireMonatge[WeaponIndex]);

			CurrentAmmoCount.Broadcast(CurrentWeapon->GetWeaponData().CurrentAmmo);
		}

		else
			return;
	}
	
	else
		return;
}

void APlayerCharacter::Reload()
{
	if (CurrentWeapon->GetCanReload() && CurrentWeapon->GetWeaponData().CurrentTotalAmmo > 0)
	{
		if (!CurrentWeapon->GetIsReloading() && !bIsChangingWeapon)
		{
			if (!CurrentWeapon->HasFullMag())
			{
				CurrentWeapon->WeaponReload();

				ReloadAnimation();

				CurrentWeapon->SetCanFire(false);

				AmmoCountReload.Broadcast(CurrentWeapon->GetWeaponData().CurrentAmmo, CurrentWeapon->GetWeaponData().CurrentTotalAmmo);
			}
		}
	}
}

void APlayerCharacter::ReloadAnimation()
{
	int32 WeaponIndex = static_cast<int32> (CurrentWeapon->GetCurrentWeaponName());

	if (CurrentWeapon->GetCurrentWeaponClass() != EWeaponClass::EWC_Shotgun)
	{
		if (ReloadMonatge.IsValidIndex(WeaponIndex))
			Instance->Montage_Play(ReloadMonatge[WeaponIndex]);
	}

	else
		CurrentWeapon->ShotgunReload();
}

void APlayerCharacter::InterpCapsuleHalfHeight(float DeltaTime)
{
	float TargetCapsuleHalfHeight{ };

	if (bIsCrouching)
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;

	else
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;

	const float InterpHalfHeight{ FMath::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), TargetCapsuleHalfHeight, DeltaTime, 20.F) };
	const float DeltaHalfHeight{ InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() };

	const FVector MeshOffset{ 0.F, 0.F, -DeltaHalfHeight };

	Arms->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void APlayerCharacter::ScanForPickups()
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetComponentRotation().Vector() * 400.F);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(CurrentWeapon);
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bIsInteractable = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bIsInteractable)
	{
		if (HitResult.Actor.Get())
		{
			if (HitResult.Actor.Get()->GetClass()->ImplementsInterface(UInteract::StaticClass()))
				IInteract::Execute_InteractableFound(HitResult.Actor.Get());
		}
	}
}

void APlayerCharacter::Interact()
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetComponentRotation().Vector() * 400.F);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(CurrentWeapon);
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bCanInteract = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bCanInteract)
	{
		if (HitResult.Actor.Get())
		{
			if (HitResult.Actor.Get()->GetClass()->ImplementsInterface(UInteract::StaticClass()))
			{
				IInteract::Execute_Interaction(HitResult.Actor.Get());
			}
		}
	}
}

bool APlayerCharacter::SpawnWeapon(TSubclassOf<class AWeaponBase> WeaponToSpawn)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();

	switch (WeaponSlotEnum)
	{
	case EWeaponSlot::EWS_First_Slot:

		if (!bIsFirstSlotFull)
		{
			WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, Location, Rotation, Params);

			if (IsValid(WeaponSlot_01))
			{
				WeaponSlot_01->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponSlot_01->GetSocketName());

				bIsFirstSlotFull = true;

				CurrentWeapon = WeaponSlot_01;

				HasWeaponEnum = EHasWeapon::EHW_HasWeapon;

				return true;
			}
		}

		else if (!bIsSecondSlotFull)
		{
			WeaponSlotEnum = EWeaponSlot::EWS_Second_Slot;

			SpawnWeapon(WeaponToSpawn);

			return true;
		}

		else
			return false;

		break;

	case EWeaponSlot::EWS_Second_Slot:

		if (!bIsSecondSlotFull)
		{
			WeaponSlot_02 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, Location, Rotation, Params);

			if (IsValid(WeaponSlot_02))
			{
				WeaponSlot_02->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
					WeaponSlot_02->GetSocketName());

				bIsSecondSlotFull = true;

				CurrentWeapon = WeaponSlot_02;

				HasWeaponEnum = EHasWeapon::EHW_HasWeapon;

				return true;
			}
		}

		else if (!bIsFirstSlotFull)
		{
			WeaponSlotEnum = EWeaponSlot::EWS_First_Slot;

			SpawnWeapon(WeaponToSpawn);

			return true;
		}

		else
			return false;

		break;

	case EWeaponSlot::EWS_Third_Slot:
		break;

	default:
		break;
	}

	return false;
}

bool APlayerCharacter::SwapWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	if (IsValid(CurrentWeapon))
	{
		if (!CurrentWeapon->GetIsFiring() && !CurrentWeapon->GetIsReloading() && !bIsChangingWeapon)
		{
			if (CurrentWeapon == WeaponSlot_01)
			{
				CurrentWeapon->Destroy();

				bIsFirstSlotFull = false;

				CurrentWeapon = WeaponSlot_02;

				SpawnWeapon(WeaponToSpawn);

				AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());

				NewWeaponUpdate.Broadcast(CurrentWeapon->GetWeaponData().WeaponIcon, CurrentWeapon->GetNameOfWeapon(), CurrentWeapon->GetCrosshairIndex());

				return true;
			}

			else
			{
				CurrentWeapon->Destroy();

				bIsSecondSlotFull = false;

				CurrentWeapon = WeaponSlot_01;

				SpawnWeapon(WeaponToSpawn);

				AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());

				NewWeaponUpdate.Broadcast(CurrentWeapon->GetWeaponData().WeaponIcon, CurrentWeapon->GetNameOfWeapon(), CurrentWeapon->GetCrosshairIndex());

				return true;
			}
		}

		else
			return false;
	}

	return false;
}

void APlayerCharacter::EquipWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		if (WeaponSlot_02 == nullptr || WeaponSlot_01 == nullptr)
			return;

		if (CurrentWeapon == WeaponSlot_01)
		{
			WeaponSlot_02->SetActorHiddenInGame(false);

			WeaponSlot_01->SetActorHiddenInGame(true);

			CurrentWeapon = WeaponSlot_02;

			AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());

			NewWeaponUpdate.Broadcast(CurrentWeapon->GetWeaponData().WeaponIcon, CurrentWeapon->GetNameOfWeapon(), CurrentWeapon->GetCrosshairIndex());
		}

		else
		{
			WeaponSlot_01->SetActorHiddenInGame(false);

			WeaponSlot_02->SetActorHiddenInGame(true);

			CurrentWeapon = WeaponSlot_01;

			AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());

			NewWeaponUpdate.Broadcast(CurrentWeapon->GetWeaponData().WeaponIcon, CurrentWeapon->GetNameOfWeapon(), CurrentWeapon->GetCrosshairIndex());
		}
	}
}

void APlayerCharacter::ShowWeapon()
{
	if (CurrentWeapon == WeaponSlot_02)
	{
		WeaponSlot_02->SetActorHiddenInGame(false);

		WeaponSlot_01->SetActorHiddenInGame(true);

		CurrentWeapon = WeaponSlot_02;

		AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());

		NewWeaponUpdate.Broadcast(CurrentWeapon->GetWeaponData().WeaponIcon, CurrentWeapon->GetNameOfWeapon(), CurrentWeapon->GetCrosshairIndex());

		CurrentAmmoCount.Broadcast(CurrentWeapon->GetWeaponData().CurrentAmmo);
	}

	else
	{
		WeaponSlot_02->SetActorHiddenInGame(true);

		WeaponSlot_01->SetActorHiddenInGame(false);

		CurrentWeapon = WeaponSlot_01;

		AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());

		NewWeaponUpdate.Broadcast(CurrentWeapon->GetWeaponData().WeaponIcon, CurrentWeapon->GetNameOfWeapon(), CurrentWeapon->GetCrosshairIndex());

		CurrentAmmoCount.Broadcast(CurrentWeapon->GetWeaponData().CurrentAmmo);
	}
}

void APlayerCharacter::SetHealth(int32 Amount)
{
	CurrentHealth += Amount;

	NewHealthAmount.Broadcast(CurrentHealth + Amount);

	if (CurrentHealth >= MaxHealth)
		CurrentHealth = MaxHealth;
}

APlayerCharacter* APlayerCharacter::GetPlayerRef_Implementation() { return this; }

void APlayerCharacter::StopFire() { CurrentWeapon->StopFire(); }

bool APlayerCharacter::HasFullHealth() { return CurrentHealth >= MaxHealth; }

bool APlayerCharacter::CanSwitchWeapons() { return bIsFirstSlotFull = bIsSecondSlotFull; }

bool APlayerCharacter::HasOpenSlot() { return !bIsFirstSlotFull || !bIsSecondSlotFull; }

