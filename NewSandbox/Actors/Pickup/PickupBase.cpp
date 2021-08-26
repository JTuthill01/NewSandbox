#include "PickupBase.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Interfaces/Widgets/Pickup/WidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NewSandbox/Widgets/Pickup/PickupWidget.h"
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

void APickupBase::Interaction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.F, FColor::Purple, __FUNCTION__);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void APickupBase::InteractableFound_Implementation()
{
	switch (Type)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		if (Player->HasOpenSlot())
			Cast<IWidgetInterface>(Player->GetPickupWidget())->UpdatePickupWidget(ItemName, IconTexture);

		//else 

		break;

	case EPickupType::EPT_Health:
		break;

	case EPickupType::EPT_Ammo:
		break;

	default:
		break;
	}
}

