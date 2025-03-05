// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SpaceInvader3DHUD.h"
#include "HUD/SpaceInvader3DOverlay.h"

void ASpaceInvader3DHUD::BeginPlay() {
	Super::BeginPlay();
	if (TObjectPtr<UWorld> World = GetWorld()) {
		if (SpaceInvader3DOverlayClass) {
			if (TObjectPtr<APlayerController> Controller = World->GetFirstPlayerController()) {
				SpaceInvader3DOverlay = CreateWidget<USpaceInvader3DOverlay>(Controller, SpaceInvader3DOverlayClass);
				SpaceInvader3DOverlay->AddToViewport();
			}
		}
	}
}
