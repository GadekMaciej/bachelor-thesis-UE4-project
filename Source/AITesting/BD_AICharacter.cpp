// Fill out your copyright notice in the Description page of Project Settings.


#include "BD_AICharacter.h"

// Sets default values
ABD_AICharacter::ABD_AICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABD_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABD_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABD_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

