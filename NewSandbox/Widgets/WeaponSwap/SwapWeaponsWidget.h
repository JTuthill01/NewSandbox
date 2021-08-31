#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSandbox/Interfaces/Widgets/Swap/SwapWeaponInterface.h"
#include "SwapWeaponsWidget.generated.h"

UCLASS()
class NEWSANDBOX_API USwapWeaponsWidget : public UUserWidget, public ISwapWeaponInterface
{
	GENERATED_BODY()
	
public:
	USwapWeaponsWidget(const FObjectInitializer& ObjectInitializer);

	void SetSwapWeapon_Implementation(class UTexture2D* SwapIcon, class UTexture2D* CurrentIcon, FName SwapText, FName CurrentText) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* CurrentWeaponIcon;

	UPROPERTY(meta = (BindWidget))
	class UImage* SwapWeaponIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentWeaponText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SwapWeaponText;

protected:
	virtual void NativeConstruct() override;

private:
	FVector2D GetScreenSize();

	FVector2D ScreenSize;
};
