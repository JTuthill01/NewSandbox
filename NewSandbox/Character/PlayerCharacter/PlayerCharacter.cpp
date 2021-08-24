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

bool APlayerCharacter::SpawnInitialWeapon(TSubclassOf<class AWeaponBase> WeaponToSpawn)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();

	WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, Location, Rotation, Params);

	if (IsValid(WeaponSlot_01))
	{
		WeaponSlot_01->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponSlot_01->GetSocketName());

		CurrentWeapon = WeaponSlot_01;

		CurrentWeapon->SetIsFirstSlotFull(true);

		HasWeaponEnum = EHasWeapon::EHW_HasWeapon;

		return true;
	}

	return false;
}

void APlayerCharacter::SwapWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		if (!CurrentWeapon->GetIsReloading() && !CurrentWeapon->GetIsFiring() && !CurrentWeapon->GetIsFiring())
		{
			if (CurrentWeapon == WeaponSlot_01)
			{
				WeaponSlot_01->SetActorHiddenInGame(true);

				WeaponSlot_02->SetActorHiddenInGame(false);

				CurrentWeapon = WeaponSlot_02;

				AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());
			}

			else if (CurrentWeapon == WeaponSlot_02)
			{
				WeaponSlot_02->SetActorHiddenInGame(true);

				WeaponSlot_01->SetActorHiddenInGame(false);

				CurrentWeapon = WeaponSlot_01;

				AnimationName.Broadcast(CurrentWeapon->GetCurrentWeaponName());
			}
		}
	}
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

bool APlayerCharacter::CanSwitchWeapons()
{
	if (CurrentWeapon->GetIsFirstSlotFull() && CurrentWeapon->GetIsSecondSlotFull())
		return true;

	return false;
}

bool APlayerCharacter::HasOpenSlot()
{
	if (!CurrentWeapon->GetIsFirstSlotFull() || !CurrentWeapon->GetIsSecondSlotFull())
		return true;

	return false;
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
	int32 WeaponIndex = static_cast<int32> (CurrentWeapon->GetCurrentWeaponName());

	if (CurrentWeapon->GetCanReload() && CurrentWeapon->GetWeaponData().CurrentAmmo > 0)
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

	FVector StartVector = Camera->GetComponentLocation();
	FVector EndVector = StartVector + (Camera->GetForwardVector() * 400.F);

	FCollisionQueryParams Parms;
	Parms.AddIgnoredActor(this);

	const bool bIsInteractable = GetWorld()->LineTraceSingleByChannel(HitResult, StartVector, EndVector, ECC_WorldStatic, Parms);

	if (bIsInteractable)
	{
		if (HitResult.Actor.Get())
		{
			if (IInteract* iTemp = Cast<IInteract>(HitResult.Actor.Get()))
			{
				if (HitResult.Actor.Get()->GetClass()->ImplementsInterface(UInteract::StaticClass()))
				{
					iTemp->Execute_OnBeginInteract(HitResult.Actor.Get());
				}
			}
		}
	}

	else
		RemovePickupWidget.Broadcast();
}

void APlayerCharacter::CheckForInteraction(AActor* HitActor, FHitResult& HitResult)
{
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
		/*HitActor = HitResult.Actor.Get();

		IInteract* iTemp = Cast<IInteract>(HitActor);
		if (iTemp)
		{
			if (HitActor)
			{
				if (HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
				{
					iTemp->Execute_OnInteract(HitActor);
				}
			}
		}*/
	}
}

void APlayerCharacter::Interact()
{
	FHitResult HitResult;

	AActor* TempActor = HitResult.Actor.Get();

	CheckForInteraction(TempActor, HitResult);
}

void APlayerCharacter::Initialize()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	Instance = Arms->GetAnimInstance();

	GetWorldTimerManager().SetTimer(PickupTimerHandle, this, &APlayerCharacter::ScanForPickups, PickupTimer, true);
}

APlayerCharacter* APlayerCharacter::GetPlayerRef_Implementation() { return this; }

void APlayerCharacter::StopFire() { CurrentWeapon->StopFire(); }

bool APlayerCharacter::HasFullHealth() { return CurrentHealth >= MaxHealth; }
