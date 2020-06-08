// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BDWeaponBase.h"
#include "BDWeaponHitScan.generated.h"

class UParticleSystem;
/**
 * 
 */
UCLASS()
class AITESTING_API ABDWeaponHitScan : public ABDWeaponBase
{
	GENERATED_BODY()
public:
	ABDWeaponHitScan();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int ProjectilesFired;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Weapon")
	float DamagePerPellet;

	//TArray <FHitResult> HitRegistered;
	FHitResult HitRegistered;
protected:
	bool IsHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
	TSubclassOf<UParticleSystem> ImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
	TSubclassOf<UParticleSystem> MuzzleEffect;

	FVector AllignCrosshairAim();
	void DetermineHit(FVector CrosshairPosition);
	UFUNCTION(Blueprintcallable, Category = "Weapon")
	virtual void Fire() override;

};
