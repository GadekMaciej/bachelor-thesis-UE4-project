// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractBase.h"
#include "BDWeaponBase.h"
#include "BDCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

// Sets default values
AInteractBase::AInteractBase() :
	TypeOfItem(EInteractableType::ITE_Weapon)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CollisionCapsule);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(CollisionCapsule);
}

// Called when the game starts or when spawned
void AInteractBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractBase::OnInteract_Implementation(AActor * caller)
{
	ABDCharacter* Picker = Cast<ABDCharacter>(caller);
	switch (TypeOfItem)
	{
	case EInteractableType::ITE_Weapon:
		Picker->PickUpWeapon(WeaponType);
		break;
	case EInteractableType::ITE_HealthPack:
		Picker->PickUpHealthPack(Intensity);
		break;
	case EInteractableType::ITE_AmmoPack:
		Picker->PickUpAmmoPack(WeaponType);
		break;
	default:
		break;
	}
	this->Destroy();
}

void AInteractBase::StartFocus_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("START FOCUS"));
	}
}

void AInteractBase::EndFocus_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("END FOCUS"));
	}
}

