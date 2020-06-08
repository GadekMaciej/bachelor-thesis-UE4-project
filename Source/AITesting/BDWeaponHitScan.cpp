// Fill out your copyright notice in the Description page of Project Settings.

#include "BDWeaponHitScan.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystem.h"
#include "BDHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "BDCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

ABDWeaponHitScan::ABDWeaponHitScan() :
	Range(500.0f),
	ProjectilesFired(1),
	DamagePerPellet(40),
	MuzzleSocketName("WeaponMuzzleSocket")
{

}

FVector ABDWeaponHitScan::AllignCrosshairAim()
{
	FVector ReturnType;
	FVector CameraLocation;
	FVector CameraForwardVector;
	FHitResult HitRes;
	FCollisionQueryParams TraceParams;
	FVector End;
	ABDCharacter* CharacterPawn = Cast<ABDCharacter>(OwningPawn);
	if (CharacterPawn)
	{
		CameraLocation = CharacterPawn->CameraCom->GetComponentLocation();
		CameraForwardVector = CharacterPawn->CameraCom->GetForwardVector();
	}
	End = (CameraForwardVector * Range) + CameraLocation;
	bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, CameraLocation, End, ECC_Visibility, TraceParams);
	///////////
	// debug //
	DrawDebugLine(GetWorld(), CameraLocation, End, FColor::Green, false, 2.0f);
	if (Hit)
	{
		ReturnType = HitRes.ImpactPoint;
		DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(10.f, 10.f, 10.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);
	}
	else
	{
		ReturnType = End;
		DrawDebugBox(GetWorld(), End, FVector(10.f, 10.f, 10.f), FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
	}
	// debug //
	///////////
	return ReturnType;
}

void ABDWeaponHitScan::DetermineHit(FVector CrosshairPosition)
{
	for (int i = 0; i < ProjectilesFired; i++)
	{

		FTransform Transform = StaticMesh->GetSocketTransform(MuzzleSocketName);
		FVector StartLocation = Transform.GetLocation();
		FHitResult HitRes;
		FCollisionQueryParams TraceParams;
		
		FVector MuzzleToHit = UKismetMathLibrary::GetDirectionUnitVector(StartLocation, CrosshairPosition);
		FVector EndLocation = MuzzleToHit * Range + StartLocation;
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.0f);

		bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, StartLocation, EndLocation, ECC_Visibility, TraceParams);


		if (Hit)
		{
			HitRegistered = HitRes;
				FString name = HitRegistered.GetActor()->GetName();
				//debug
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, name);
				//
				UGameplayStatics::ApplyDamage(HitRegistered.GetActor(), DamagePerPellet, OwningPawn->GetController(), this, DamageType);
				DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(30.f, 30.f, 30.f), FColor::Yellow, false, 8.f, ECC_WorldStatic, 1.f);
		}
	}
}


void ABDWeaponHitScan::Fire()
{
	ABDWeaponBase::Fire();
	DetermineHit(AllignCrosshairAim());
}
