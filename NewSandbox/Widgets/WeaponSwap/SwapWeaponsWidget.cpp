#include "SwapWeaponsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

USwapWeaponsWidget::USwapWeaponsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void USwapWeaponsWidget::SetSwapWeapon_Implementation(class UTexture2D* SwapIcon, class UTexture2D* CurrentIcon, FName SwapText, FName CurrentText)
{
	SwapWeaponIcon->SetBrushFromTexture(SwapIcon);

	CurrentWeaponIcon->SetBrushFromTexture(CurrentIcon);

	SwapWeaponText->SetText(FText::FromName(SwapText));

	CurrentWeaponText->SetText(FText::FromName(CurrentText));
}

void USwapWeaponsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetDesiredSizeInViewport(FVector2D(400.F, 400.F));

	ScreenSize = FVector2D(GetScreenSize().X / 2, GetScreenSize().Y / 2);

	SetPositionInViewport(ScreenSize);
}

FVector2D USwapWeaponsWidget::GetScreenSize()
{
	FVector2D Result = FVector2D(0.F);

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);

	return Result;
}
