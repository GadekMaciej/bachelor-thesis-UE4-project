// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BDProjectileBase.h"
#include "BDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UCharacterMovementComponent;
class ABDWeaponBase;

UCLASS()
class AITESTING_API ABDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABDCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmCom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraCom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaximumHealth;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Health|Shield")
	float CurrentShield;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Health|Shield")
	float MaximumShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield")
	float ShieldRegenerateDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield")
	float ShieldRegenTickDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield")
	float ShieldRegenAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AICustom")
	int AlliesCount;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void Die();
	UFUNCTION()
	void PickUpWeapon(TSubclassOf<ABDWeaponBase>& WeaponType);
	UFUNCTION()
	void PickUpHealthPack(float Intensity);
	UFUNCTION()
	void PickUpAmmoPack(TSubclassOf<ABDWeaponBase>& WeaponType);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bIsRecentlyDamaged;
	FTimerHandle TimerHandleShieldRegen;
	FTimerHandle TimerHandleStartShieldRegen;

	UFUNCTION()
	bool CheckForCurrentWeapon();
	UFUNCTION()
	void Heal(float Intensity);
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void TurnRight(float Value);
	UFUNCTION()
	void TurnAtRate(float Value);
	UFUNCTION()
	void LookUpAtRate(float Value);
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void CheckHealth();
	UFUNCTION()
	bool CheckForShield();
	UFUNCTION()
	void StartRegenerateShield();
	UFUNCTION(BlueprintCallable, Category = "Player")
	void Fire();
	void Crouch();
	void Sprint();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TraceRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "gameplay")
	FVector MuzzleOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABDProjectileBase> ProjectileClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	ABDWeaponBase* CurrentWeapon;
	TSharedPtr<ABDWeaponBase> CurrentWeaponSP;
	//////////////////////
	//DEBUG PURPOSE ONLY//
	//UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Weapon")
	//TSubclassOf<ABDWeaponBase> WeaponToSpawn;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	//TSubclassOf<class UUserWidget> PlayerCharacterInterface;
	//DEBUG PURPOSE ONLY//
	//////////////////////

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void FireWeapon();
	void FireWeapon_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void RegenerateShield();
	void RegenerateShield_Implementation();
	UFUNCTION(BlueprintPure)
	FString DetermineHealth();
	UFUNCTION(BlueprintPure)
	FString DetermineShield();
	UFUNCTION(BlueprintPure)
	FString DetermineAllies();


	class UMyUserWidget* UserInterface;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AActor* FocusedActor;
};
