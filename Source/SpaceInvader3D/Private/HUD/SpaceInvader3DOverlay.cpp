// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SpaceInvader3DOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USpaceInvader3DOverlay::SetHealthBarPercent(const float& Percent) {
	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(Percent);
	}
}

void USpaceInvader3DOverlay::SetHeadLightStatusToOn() {
	if (HeadLightStatus) {
		HeadLightStatus->SetText(FText::FromString(TEXT("HeadLights: ON")));
	}
}

void USpaceInvader3DOverlay::SetHeadLightStatusToOff() {
	if (HeadLightStatus) {
		HeadLightStatus->SetText(FText::FromString(TEXT("HeadLights: OFF")));
	}
}
