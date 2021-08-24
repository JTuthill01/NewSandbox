#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Interactables/Pickup/PickupBase.h"
#include "ItalianPickup.generated.h"

UCLASS()
class NEWSANDBOX_API AItalianPickup : public APickupBase
{
	GENERATED_BODY()

public:
	AItalianPickup();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
