#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SwapWeaponWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class NEWSANDBOX_API USwapWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetWeaponText(FText Name);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WeaponOneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WeaponTwoText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* WeaponOneImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* WeaponTwoImage;

	class APlayerCharacter* Player;

protected:
	virtual void NativeOnInitialized() override;
};
