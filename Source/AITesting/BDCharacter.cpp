// Fill out your copyright notice in the Description page of Project Settings.

#include "BDCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BDHealthComponent.h"
#include "Engine/Engine.h"
#include "BDWeaponBase.h"
#include "BDWeaponHitScan.h"
#include "MyUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interactable.h"
#include "DTreeT.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABDCharacter::ABDCharacter() 
	: BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	TraceRange(2000)
{

	SpringArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmCom->SetupAttachment(RootComponent);

	CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraCom->SetupAttachment(SpringArmCom);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	UCharacterMovementComponent* CharacterMovementComponent = this->GetCharacterMovement();
	CharacterMovementComponent->bOrientRotationToMovement = true;

	PrimaryActorTick.bCanEverTick = true;
}

void ABDCharacter::CheckHealth()
{
	if (CurrentHealth <= 0)
	{
		Die();
	}
}

bool ABDCharacter::CheckForShield()
{
	if (CurrentShield > 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ABDCharacter::RegenerateShield_Implementation()
{
	if (CurrentShield == MaximumShield)
	{
		GetWorldTimerManager().ClearTimer(TimerHandleShieldRegen);
		//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, TEXT("ShieldFull"), false);
		return;
	}
	CurrentShield = FMath::Clamp(ShieldRegenAmount + CurrentShield, 0.0f, MaximumShield);
	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Regenerated NEW!!!!!!!"), false);
}

void ABDCharacter::StartRegenerateShield()
{
	GetWorldTimerManager().SetTimer(TimerHandleShieldRegen, this, &ABDCharacter::RegenerateShield, ShieldRegenTickDelay, true);
	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan, TEXT("StartRegenerating"), false);
}

void ABDCharacter::TakeDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	GetWorldTimerManager().ClearTimer(TimerHandleStartShieldRegen);
	GetWorldTimerManager().ClearTimer(TimerHandleShieldRegen);
	float RemainingDamage = Damage;
	if (Damage <= 0.0f)
	{
		return;
	}
	if (CheckForShield())
	{
		bool bPiercedShield = false;
		if (CurrentShield < Damage)
		{
			RemainingDamage = CurrentShield - Damage;
			bPiercedShield = true;
		}
		CurrentShield = FMath::Clamp(CurrentShield - Damage, 0.0f, MaximumShield);
		if (!bPiercedShield)
		{
			GetWorldTimerManager().SetTimer(TimerHandleStartShieldRegen, this, &ABDCharacter::StartRegenerateShield, ShieldRegenerateDelay, false);
			return;
		}
	}
	CurrentHealth = FMath::Clamp(CurrentHealth - RemainingDamage, 0.0f, MaximumHealth);
	CheckHealth();
	GetWorldTimerManager().SetTimer(TimerHandleStartShieldRegen, this, &ABDCharacter::StartRegenerateShield, ShieldRegenerateDelay, false);
}

// Called when the game starts or when spawned
void ABDCharacter::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &ABDCharacter::OnOverlapBegin);
	this->OnTakeAnyDamage.AddDynamic(this, &ABDCharacter::TakeDamage);
	

	///////////////////////////////////////
	//SPawning gun and attaching to actor//
	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	FTransform SocketTransform = SkeletalMesh->GetSocketTransform(FName("hand_rSocket"));
	//	FActorSpawnParameters DefaultParams;
	//	CurrentWeapon = World->SpawnActor<ABDWeaponBase>(WeaponToSpawn, SocketTransform, DefaultParams);
	//	EAttachmentRule myrule = EAttachmentRule::SnapToTarget;
	//	FAttachmentTransformRules mytransformrules(myrule, true);
	//	CurrentWeapon->AttachToComponent(SkeletalMesh, mytransformrules, FName("hand_rSocket"));
	//}
	////SPawning gun and attaching to actor//
	///////////////////////////////////////
}

void ABDCharacter::MoveForward(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABDCharacter::TurnRight(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABDCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
}

void ABDCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
}

void ABDCharacter::Interact()
{
	TraceForward();

	if (FocusedActor)
	{
		IInteractable* Interface = Cast<IInteractable>(FocusedActor);
		if (Interface)
		{
			Interface->Execute_OnInteract(FocusedActor, this);
		}
	}
}


void ABDCharacter::TraceForward_Implementation()
{
	FVector Locat;
	FRotator Rotat;
	FHitResult HitRes;

	GetController()->GetPlayerViewPoint(Locat, Rotat);

	FVector Start = Locat;
	FVector End = Locat + (Rotat.Vector() * TraceRange);
	FCollisionQueryParams TraceParams;
	bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, Start, End, ECC_Visibility, TraceParams);
	//debug
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);

	if (Hit)
	{
		AActor* Interactable = HitRes.GetActor();
		//DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(10.f, 10.f, 10.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);
		if (Interactable)
		{
			if (Interactable != FocusedActor)
			{
				if (FocusedActor)
				{
					IInteractable* Interface = Cast<IInteractable>(FocusedActor);
					if (Interface)
					{
						Interface->Execute_EndFocus(FocusedActor);
					}
				}
				IInteractable* Interface = Cast<IInteractable>(Interactable);
				if (Interface)
				{
					Interface->Execute_StartFocus(Interactable);
				}
				FocusedActor = Interactable;
			}
		}
		else
		{
			if (FocusedActor)
			{
				IInteractable* Interface = Cast<IInteractable>(FocusedActor);
				if (Interface)
				{
					Interface->Execute_EndFocus(FocusedActor);
				}
			}
			FocusedActor = nullptr;
		}
	}
}

void ABDCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	{
		//GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Emerald, TEXT("OVERLAPBEGIN!!"));
		IInteractable* Interface = Cast<IInteractable>(OtherActor);
		if (Interface)
		{
			Interface->Execute_OnInteract(OtherActor, this);
		}
	}
}

bool ABDCharacter::CheckForCurrentWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ABDCharacter::PickUpWeapon(TSubclassOf<ABDWeaponBase>& WeaponType)
{
	//Deleting previous weapon.
	if (CheckForCurrentWeapon())
	{
		CurrentWeapon->Destroy();
	}
	UWorld* World = GetWorld();
	if (World)
	{
		FTransform SocketTransform = SkeletalMesh->GetSocketTransform(FName("hand_rSocket"));
		FActorSpawnParameters DefaultParams;
		CurrentWeapon = World->SpawnActor<ABDWeaponBase>(WeaponType, SocketTransform, DefaultParams);
		FAttachmentTransformRules mytransformrules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(SkeletalMesh, mytransformrules, FName("hand_rSocket"));
	}
}

void ABDCharacter::Heal(float Intensity)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Intensity, 0.0f, MaximumHealth);
}

void ABDCharacter::PickUpHealthPack(float Intensity)
{
	Heal(Intensity);
}

void ABDCharacter::PickUpAmmoPack(TSubclassOf<ABDWeaponBase>& WeaponType)
{
	
}

void ABDCharacter::Fire()
{
	if (ProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector MuzzleLocation = CameraLocation + this->GetActorForwardVector() * 50;
			//FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		UWorld* World = GetWorld();
		
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			ABDProjectileBase* Projectile = World->SpawnActor<ABDProjectileBase>(ProjectileClass, MuzzleLocation, CameraRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}

	}
}


void ABDCharacter::FireWeapon_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void ABDCharacter::Die()
{
	if(CurrentWeapon)
	CurrentWeapon->Destroy();
	this->Destroy();
}

FString ABDCharacter::DetermineHealth()
{
	if (CurrentHealth > MaximumHealth / 2)
	{
		return FString("MoreThanHalf");
	}
	else
	{
		return FString("LessThanHalf");
	}
}

FString ABDCharacter::DetermineShield()
{
	if (CurrentShield > MaximumShield / 2)
	{
		return FString("MoreThanHalf");
	}
	else
	{
		return FString("LessThanHalf");
	}
}

FString ABDCharacter::DetermineAllies()
{
	if (AlliesCount < 2)
	{
		return FString("None");
	}
	else
	{
		return FString("Many");
	}
}



// Called every frame
void ABDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForward();
}

// Called to bind functionality to input
void ABDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABDCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABDCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABDCharacter::TurnRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABDCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ABDCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABDCharacter::FireWeapon);
	PlayerInputComponent->BindAction("FireAlt", IE_Pressed, this, &ABDCharacter::Fire);
}

