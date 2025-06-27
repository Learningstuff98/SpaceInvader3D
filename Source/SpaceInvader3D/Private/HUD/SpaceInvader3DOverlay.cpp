// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/SpaceInvader3DOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USpaceInvader3DOverlay::SetMissileReloadingProgress(const float& Percent) {
	if (MissileReloadingProgressBar) MissileReloadingProgressBar->SetPercent(Percent);
}

void USpaceInvader3DOverlay::SetHealthBarPercent(const float& Percent) {
	if (HealthProgressBar) HealthProgressBar->SetPercent(Percent);
}

void USpaceInvader3DOverlay::SetHeadLightText(const bool& HeadLightsAreOn) {
	if (HeadLightText) {
		if (HeadLightsAreOn) {
			HeadLightText->SetText(FText::FromString(TEXT("HeadLights: ON")));
		}
		else {
			HeadLightText->SetText(FText::FromString(TEXT("")));
		}
	}
}
