#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateBrush.h"
#include "PickupWidget.generated.h"

UCLASS()
class NEWSANDBOX_API UPickupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPickupWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION()
	void SetPickupName(FText Name);

	UFUNCTION(BlueprintCallable)
	void SetPickupIcon(class UTexture2D* Icon);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PickupText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UImage* IconImage;

protected:
	virtual void NativeConstruct() override;

private:
	FVector2D GetScreenSize();

	FVector2D ScreenSize;
};
