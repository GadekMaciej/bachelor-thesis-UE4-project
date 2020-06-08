// Fill out your copyright notice in the Description page of Project Settings.
#include "BDHealthComponent.h"
#include "BDCharacter.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UBDHealthComponent::UBDHealthComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaximumHealth = 100;
	Health = MaximumHealth;
	// ...
}

void UBDHealthComponent::Die()
{
	if (Health <= 0)
	{
		ABDCharacter* Owner = Cast<ABDCharacter>(GetOwner());
		if (Owner)
		{
			Owner->Die();
		}
	}
}


void UBDHealthComponent::TakeDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, MaximumHealth);
	Die();
}

// Called when the game starts
void UBDHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UBDHealthComponent::TakeDamage);
	}
	
}


// Called every frame
void UBDHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

