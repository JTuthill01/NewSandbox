#include "PickupBase.h"
#include "Components/WidgetComponent.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Widgets/SwapWeapon/SwapWeaponWidget.h"
#include "NewSandbox/Widgets/Pickup/PickupWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

// Sets default values
APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetGenerateOverlapEvents(true);

	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	PickupWidget = CreateWidget<UPickupWidget>(GetWorld(), WidgetToSpawn);

	if (Player)
		Player->RemovePickupWidget.AddUniqueDynamic(this, &APickupBase::Remove);
}

void APickupBase::Tick(float DeltaTime)
{
	if (PickupWidget)
	{
		PickupWidget->SetPickupName(ItemName);

		PickupWidget->SetPickupIcon(IconTexture);
	}
}

void APickupBase::Remove()
{
	if (PickupWidget)
	{ 
		if (PickupWidget->IsInViewport())
			PickupWidget->RemoveFromParent();
	}

	else
		return;
}

void APickupBase::EquipWeapon()
{
	auto Weapon = Player->GetCurrentWeapon()->SpawnWeapon(Player, CurrentWeapon, WeaponSlot1, WeaponSlot2, WeaponToEquip, EWeaponSlot::EWS_First_Slot, EHasWeapon::EHW_HasWeapon);

	Player->SetCurrentWeapon(Weapon);
}

void APickupBase::SetWidget()
{
	switch (PickupType)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		if (Player->HasOpenSlot())
		{
			if (PickupWidget)
			{
				if (!PickupWidget->IsInViewport())
					PickupWidget->AddToViewport(0);
			}
		}

		break;

	case EPickupType::EPT_Health:

		if (!Player->HasFullHealth())
		{
			break;
		}

		break;

	case EPickupType::EPT_Ammo:

		if (!Player->GetCurrentWeapon()->IsAmmoFull())
		{
			break;
		}

		break;

	default:
		break;
	}
}

void APickupBase::OnBeginInteract_Implementation()
{
	SetWidget();
}

