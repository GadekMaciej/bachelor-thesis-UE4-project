// Fill out your copyright notice in the Description page of Project Settings.

#include "BDProjectileBase.h"
#include "BDCharacter.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ABDProjectileBase::ABDProjectileBase() :
	ProjectileSpeed(100.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(10.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	//CollisionComponent->OnComponentHit.AddDynamic(this, &ABDProjectileBase::OnHit);
	RootComponent = CollisionComponent;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	InitialLifeSpan = 2.0f;
}

void ABDProjectileBase::FireInDirection(const FVector & ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void ABDProjectileBase::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
		this->Destroy();
	}
}

void ABDProjectileBase::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != this)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageType);
		this->Destroy();
	}
}

// Called when the game starts or when spawned
void ABDProjectileBase::BeginPlay()
{
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABDProjectileBase::OnOverlap);
}

// Called every frame
void ABDProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

