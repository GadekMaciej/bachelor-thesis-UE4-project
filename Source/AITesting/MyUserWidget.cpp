// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUserWidget.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (MyTextBlock)
	{
		FString MojString = "Ammo";
		FText mojtekst = FText::FromString(MojString);
		MyTextBlock->SetText(mojtekst);
	}
}

//void UMyUserWidget::UpdateAmmo(int CurrentAmmo, int MaximumAmmo)
//{
//	if (ETCurrentAmmo && ETMaximumAmmo)
//	{
//		ETCurrentAmmo->SetText(FText::AsNumber(CurrentAmmo));
//		ETMaximumAmmo->SetText(FText::AsNumber(MaximumAmmo));
//	}
//}

void UMyUserWidget::UpdateHealthPercentage(float CurrentHealth, float MaximumHealth)
{
	if (ProgHealthBar)
	{
		ProgHealthBar->SetPercent(CurrentHealth / MaximumHealth);
	}
}

void UMyUserWidget::UpdateShieldPercentage(float CurrentShield, float MaximumShield)
{
	if (ProgShieldBar)
	{
		ProgShieldBar->SetPercent(CurrentShield / MaximumShield);
	}
}