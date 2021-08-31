#include "PlayerAnimInstance.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NewSandbox/Structs/Player/PlayerAnimsDataTable.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		IdleSequence = Player->PlayerAnims.IdleSequence;

		WalkSequence = Player->PlayerAnims.WalkSequence;

		Player->AnimationName.AddUniqueDynamic(this, &UPlayerAnimInstance::SetAnimation);
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (!Player)
		Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (Player)
	{
		FVector Velocity = Player->GetVelocity();

		Velocity.Z = 0.F;

		Speed = Velocity.Size();

		bIsInAir = Player->GetCharacterMovement()->IsFalling();

		if (Player->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.F)
			bIsAccelerating = true;

		else
			bIsAccelerating = false;
	}
}

void UPlayerAnimInstance::SetAnimation(EWeaponName Name)
{
	Name = Player->GetCurrentWeapon()->GetCurrentWeaponName();

	FString PlayerAnimTablePath(TEXT("DataTable'/Game/_Game/Blueprints/DataTable/DT_PlayerAnims.DT_PlayerAnims'"));

	UDataTable* PlayerAnimTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *PlayerAnimTablePath));

	if (PlayerAnimTableObject)
	{
		FPlayerAnimsDataTable* AnimRow = nullptr;

		switch (Name)
		{
		case EWeaponName::EWN_TT33:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("TT33"), TEXT(""));
			
			break;

		case EWeaponName::EWN_AK47:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("AK47"), TEXT(""));

			break;

		case EWeaponName::EWN_AmericanShotgun:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("AmericanShotgun"), TEXT(""));

			break;

		case EWeaponName::EWN_BelgianAR:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("BelgianAR"), TEXT(""));

			break;

		case EWeaponName::EWN_GermanSMG:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("GermanSMG"), TEXT(""));

			break;

		case EWeaponName::EWN_HandCannon:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("HandCannon"), TEXT(""));

			break;

		case EWeaponName::EWN_SKS:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("SKS"), TEXT(""));

			break;

		case EWeaponName::EWN_XM82:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("XM82"), TEXT(""));

			break;

		case EWeaponName::EWN_Bulldog:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("Bulldog"), TEXT(""));

			break;

		case EWeaponName::EWN_L86:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("L86"), TEXT(""));

			break;

		case EWeaponName::EWN_AK74:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("AK74"), TEXT(""));

			break;

		case EWeaponName::EWN_M4A1:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("M4A1"), TEXT(""));

			break;

		case EWeaponName::EWN_NavySMG:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("NavySMG"), TEXT(""));

			break;

		case EWeaponName::EWN_ItalianShotgun:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("ItalianShotgun"), TEXT(""));

			break;

		case EWeaponName::EWN_SVD:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("SVD"), TEXT(""));

			break;

		case EWeaponName::EWN_ShortStrokeAR:

			AnimRow = PlayerAnimTableObject->FindRow<FPlayerAnimsDataTable>(FName("ShortStrokeAR"), TEXT(""));

			break;

		default:
			break;
		}

		if (AnimRow)
		{
			IdleSequence = AnimRow->IdleSequence;

			WalkSequence = AnimRow->WalkSequence;
		}
	}
}

