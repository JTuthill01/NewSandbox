#include "PickupBase.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Interfaces/Widgets/Pickup/WidgetInterface.h"
#include "NewSandbox/Interfaces/Widgets/Swap/SwapWeaponInterface.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
APickupBase::APickupBase()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupMesh->SetGenerateOverlapEvents(true);
	PickupMesh->SetCastShadow(false);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(PickupMesh);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_Visibility);
	CollisionSphere->SetSphereRadius(40.F);

	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	Player = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APickupBase::Interaction_Implementation()
{
	switch (Type)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		if (Player->HasOpenSlot())
			Spawn();

		else if (Player->CanSwitchWeapons())
			SwapWeaponSpawn();
		
		break;

	case EPickupType::EPT_Health:

		if (Player->HasFullHealth())
			break;

		else
		{
			if (SetHealth())
				Destroy();
		}

		break;

	case EPickupType::EPT_Ammo:

		if (Player->GetCurrentWeapon()->IsAmmoFull())
			break;

		else
			if (SetAmmo())
				Destroy();

		break;

	default:
		break;
	}
}

void APickupBase::Spawn()
{
	if (Player->SpawnWeapon(WeaponToSpawn))
	{
		Destroy();

		Player->ShowWeapon();

		Player->NewWeaponUpdate.Broadcast(Player->GetCurrentWeapon()->GetWeaponData().WeaponIcon, Player->GetCurrentWeapon()->GetNameOfWeapon(), Player->GetCurrentWeapon()->GetCrosshairIndex());
	}
}

void APickupBase::SwapWeaponSpawn()
{
	if (Player->SwapWeapon(WeaponToSpawn))
	{
		Destroy();

		Player->ShowWeapon();

		Player->NewWeaponUpdate.Broadcast(Player->GetCurrentWeapon()->GetWeaponData().WeaponIcon, Player->GetCurrentWeapon()->GetNameOfWeapon(), Player->GetCurrentWeapon()->GetCrosshairIndex());
	}
}

bool APickupBase::SetHealth()
{
	if (!Player)
		return false;

	switch (HealthType)
	{
	case EPickupHealthType::EPH_None:
		return false;

	case EPickupHealthType::EPH_HealthSmall:

		Player->SetHealth(HealthPackValue);

		return true;

	case EPickupHealthType::EPH_HealthMedium:

		Player->SetHealth(HealthPackValue);

		return true;

	case EPickupHealthType::EPH_HealthLarge:

		Player->SetHealth(HealthPackValue);

		return true;

	default:
		return false;
	}

	return false;
}

bool APickupBase::SetAmmo()
{
	if (!Player)
		return false;

	switch (AmmoType)
	{
	case EPickupAmmoType::EPA_None:
		return false;

	case EPickupAmmoType::EPH_PistolAmmo:

		if (Player->GetCurrentWeapon()->GetCurrentWeaponClass() == EWeaponClass::EWC_Pistol)
		{
			Player->GetCurrentWeapon()->SetCurrentTotalAmmo(PistolPickupValue);

			return true;
		}

		return false;

	case EPickupAmmoType::EPS_RifleAmmo:

		if (Player->GetCurrentWeapon()->GetCurrentWeaponClass() == EWeaponClass::EWC_SemiAutoRifle || Player->GetCurrentWeapon()->GetCurrentWeaponClass() == EWeaponClass::EWC_FullAutoRifle)
		{
			Player->GetCurrentWeapon()->SetCurrentTotalAmmo(RiflePickupValue);

			return true;
		}

		return false;

	case EPickupAmmoType::EPA_ShotgunAmmo:

		if (Player->GetCurrentWeapon()->GetCurrentWeaponClass() == EWeaponClass::EWC_Shotgun)
		{
			Player->GetCurrentWeapon()->SetCurrentTotalAmmo(ShotgunPickupValue);

			return true;
		}

		return false;

	default:
		return false;
	}

	return false;
}

void APickupBase::InteractableFound_Implementation()
{
	switch (Type)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		if (Player->HasOpenSlot())
		{	
			
		}

		else if (Player->CanSwitchWeapons())
		{
			
		}

		break;

	case EPickupType::EPT_Health:
		
		

		break;

	case EPickupType::EPT_Ammo:

		

		break;

	default:
		break;
	}
}

