#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSandbox/Interfaces/Widgets/Pickup/WidgetInterface.h"
#include "PickupWidget.generated.h"

UCLASS(Abstract)
class NEWSANDBOX_API UPickupWidget : public UUserWidget, public IWidgetInterface
{
	GENERATED_BODY()

public:
	UPickupWidget(const FObjectInitializer& ObjectInitializer);

	 virtual void UpdatePickupWidget(FName Name, class UTexture2D* Icon) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* PickupIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PickupText;

protected:
	virtual void NativeConstruct() override;

private:
	FVector2D GetScreenSize();

	FVector2D ScreenSize;
};
