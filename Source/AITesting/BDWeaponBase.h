// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDWeaponBase.generated.h"

UCLASS()
class AITESTING_API ABDWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABDWeaponBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* StaticMesh;
	virtual void Fire();

protected:
	APawn* OwningPawn;

	bool IsCanFire();
	void SetOwningPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaximumAmmunition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int Ammunition;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
