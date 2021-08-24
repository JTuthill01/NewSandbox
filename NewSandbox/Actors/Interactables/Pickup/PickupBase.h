#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewSandbox/Enums/PickupEnums.h"
#include "NewSandbox/Interfaces/Interact/Interact.h"
#include "PickupBase.generated.h"

UCLASS()
class NEWSANDBOX_API APickupBase : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

public:
	virtual void OnBeginInteract_Implementation() override;

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void Remove();

	UFUNCTION()
	void EquipWeapon();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets)
	class UPickupWidget* PickupWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets)
	class USwapWeaponWidget* SwapWeaponWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Widgets)
	TSubclassOf<UPickupWidget> WidgetToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Widgets)
	TSubclassOf<USwapWeaponWidget> SwitchWeaponWidgetToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enums)
	EPickupType PickupType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Name)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Icon)
	class UTexture2D* IconTexture;

	UPROPERTY()
	class APlayerCharacter* Player;

	UPROPERTY()
	class AWeaponBase* CurrentWeapon;

	UPROPERTY()
	AWeaponBase* WeaponSlot1;

	UPROPERTY()
	AWeaponBase* WeaponSlot2;

	UPROPERTY()
	TSubclassOf<class AWeaponBase> WeaponToEquip;

private:
	void SetWidget();
};
