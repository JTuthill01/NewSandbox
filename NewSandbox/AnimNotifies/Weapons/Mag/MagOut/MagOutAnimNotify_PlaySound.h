#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "MagOutAnimNotify_PlaySound.generated.h"

UCLASS()
class NEWSANDBOX_API UMagOutAnimNotify_PlaySound : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
