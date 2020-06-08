// Fill out your copyright notice in the Description page of Project Settings.

#include "MyDecQu.h"
#include "DTreeT.h"

// Sets default values
AMyDecQu::AMyDecQu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyDTreeT = PrepareTree();
}

// Called when the game starts or when spawned
void AMyDecQu::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AMyDecQu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AMyDecQu::GetDecision(FString NumberOfAllies, FString CurrentHealth, FString CurrentShield)
{
	map<string, string> input;
	input.insert(make_pair("NumberOfAllies", TCHAR_TO_UTF8(*NumberOfAllies)));
	input.insert(make_pair("CurrentHealth", TCHAR_TO_UTF8(*CurrentHealth)));
	input.insert(make_pair("CurrentShield", TCHAR_TO_UTF8(*CurrentShield)));
	//input.insert(make_pair("enemystunned", "false"));
	//input.insert(make_pair("enemyhealth", "lessThanHalf"));
	string tmp = MyDTreeT->GetDecision(input);
	return tmp.c_str();
}



