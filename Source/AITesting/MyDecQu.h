// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyDecQu.generated.h"

UCLASS()
class AITESTING_API AMyDecQu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDecQu();
	UFUNCTION(BlueprintCallable, Category = "DecisionTree")
	FString GetDecision(FString NumberOfAllies, FString CurrentHealth, FString CurrentShield);
	class DTreeT* MyDTreeT;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
