// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceInvader3DOverlay.generated.h"

UCLASS()
class SPACEINVADER3D_API USpaceInvader3DOverlay : public UUserWidget
{
	GENERATED_BODY()
	
private:

	// HUD elements

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HeadLightText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> MissileReloadingProgressBar;

public:

	// Setters
	void SetMissileReloadingProgressBar(const float& Percent);
	void SetHealthBarPercent(const float& Percent);
	void SetHeadLightText(const bool& HeadLightsAreOn);
};
