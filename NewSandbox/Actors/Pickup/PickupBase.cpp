#include "PickupBase.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "NewSandbox/Interfaces/Widgets/Pickup/WidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NewSandbox/Widgets/Pickup/PickupWidget.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupMesh->SetGenerateOverlapEvents(true);

	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = IPlayerInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::InteractableFound_Implementation()
{
	switch (Type)
	{
	case EPickupType::EPT_None:
		break;

	case EPickupType::EPT_Weapon:

		Cast<IWidgetInterface>(Player->GetPickupWidget())->UpdatePickupWidget(ItemName, IconTexture);

		break;

	case EPickupType::EPT_Health:
		break;

	case EPickupType::EPT_Ammo:
		break;

	default:
		break;
	}
}

