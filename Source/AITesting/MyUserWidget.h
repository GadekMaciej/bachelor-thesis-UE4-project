// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UEditableText;
class UImage;
	/**
	 *
	 */
	UCLASS()
	class AITESTING_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMyUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock *MyTextBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgHealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgShieldBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* ETCurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableText* ETMaximumAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ICrosshair;

	//UFUNCTION(BlueprintCallable)
	//void UpdateAmmo(int CurrentAmmo, int MaximumAmmo);
	UFUNCTION(BlueprintCallable)
	void UpdateHealthPercentage(float CurrentHealth, float MaximumHealth);
	UFUNCTION(BlueprintCallable)
	void UpdateShieldPercentage(float CurrentShield, float MaximumShield);
protected:
};
