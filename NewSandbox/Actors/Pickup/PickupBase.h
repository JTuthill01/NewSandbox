#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewSandbox/Interfaces/Interact/Interact.h"
#include "NewSandbox/Enums/PickupEnums.h"
#include "PickupBase.generated.h"

UCLASS()
class NEWSANDBOX_API APickupBase : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

public:
	void InteractableFound_Implementation() override;

	void Interaction_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PickupType)
	EPickupType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Name)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Icon)
	class UTexture2D* IconTexture;

	UPROPERTY()
	class APlayerCharacter* Player;
};
