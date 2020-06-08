// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractBase.generated.h"

class ABDWeaponBase;

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	ITE_Weapon		UMETA(DisplayName = "Weapon"),
	ITE_HealthPack	UMETA(DisplayName = "HealthPack"),
	ITE_AmmoPack	UMETA(DisplayName = "AmmoPack")
};

UCLASS()
class AITESTING_API AInteractBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class UCapsuleComponent* CollisionCapsule;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "PickUp")
	EInteractableType TypeOfItem;
	UPROPERTY(EditAnywhere, Category = "PickUp|HealthPack")
	float Intensity;
	UPROPERTY(EditAnywhere, Category = "PickUp|Weapon")
	TSubclassOf<ABDWeaponBase> WeaponType;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* caller);
	virtual void OnInteract_Implementation(AActor* caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();
	virtual void StartFocus_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();
	virtual void EndFocus_Implementation();
};
