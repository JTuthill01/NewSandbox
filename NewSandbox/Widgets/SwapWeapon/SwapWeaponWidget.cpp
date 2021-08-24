#include "SwapWeaponWidget.h"
#include "NewSandbox/Character/PlayerCharacter/PlayerCharacter.h"
#include "NewSandbox/Actors/Weapons/WeaponBase/WeaponBase.h"
#include "NewSandbox/Interfaces/Player/PlayerInterface.h"
#include "Components/TextBlock.h"

void USwapWeaponWidget::NativeOnInitialized()
{
	Player = IPlayerInterface::Execute_GetPlayerRef(GetOwningPlayerPawn());
}

void USwapWeaponWidget::SetWeaponText(FText Name)
{
	 WeaponOneText->SetText(Name);	
}

