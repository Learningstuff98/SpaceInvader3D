// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SpaceInvader3DOverlay.h"
#include "Components/ProgressBar.h"

void USpaceInvader3DOverlay::SetHealthBarPercent(const float& Percent) {
	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(Percent);
	}
}
