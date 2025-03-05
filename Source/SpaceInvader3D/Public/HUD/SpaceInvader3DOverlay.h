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

	// Progress Bars

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgressBar;

public:

	// Setters
	void SetHealthBarPercent(const float& Percent);
};
