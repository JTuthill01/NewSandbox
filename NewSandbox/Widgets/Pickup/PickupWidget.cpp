#include "PickupWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UPickupWidget::UPickupWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void UPickupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetDesiredSizeInViewport(FVector2D(400.F, 400.F));

	ScreenSize = FVector2D(GetScreenSize().X / 2, GetScreenSize().Y / 2);

	SetPositionInViewport(ScreenSize);
}

FVector2D UPickupWidget::GetScreenSize()
{
	FVector2D Result = FVector2D(0.F);

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);

	return Result;
}

void UPickupWidget::SetPickupName(FText Name)
{
	if (IsValid(PickupText))
		PickupText->SetText(Name);
}

void UPickupWidget::SetPickupIcon(UTexture2D* Icon)
{
	if (IsValid(IconImage))
		IconImage->SetBrushFromTexture(Icon);
}
