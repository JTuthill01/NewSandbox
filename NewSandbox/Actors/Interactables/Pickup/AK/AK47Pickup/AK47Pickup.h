#pragma once

#include "CoreMinimal.h"
#include "NewSandbox/Actors/Interactables/Pickup/PickupBase.h"
#include "AK47Pickup.generated.h"

UCLASS()
class NEWSANDBOX_API AAK47Pickup : public APickupBase
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

};
