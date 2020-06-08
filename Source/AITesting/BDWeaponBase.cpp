// Fill out your copyright notice in the Description page of Project Settings.

#include "BDWeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABDWeaponBase::ABDWeaponBase() :
	MaximumAmmunition(30),
	Ammunition(25)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

void ABDWeaponBase::Fire()
{
	if (IsCanFire())
	{
		Ammunition = Ammunition - 1;

	}
	else
	{
	}
}

void ABDWeaponBase::SetOwningPawn()
{
	OwningPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

bool ABDWeaponBase::IsCanFire()
{
	return Ammunition > 0 ? true : false;
}

// Called when the game starts or when spawned
void ABDWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//DEBUG ONLY set pawn at spawning
	SetOwningPawn();
	
}

// Called every frame
void ABDWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

