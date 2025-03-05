// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceInvader3DHUD.generated.h"

UCLASS()
class SPACEINVADER3D_API ASpaceInvader3DHUD : public AHUD
{
	GENERATED_BODY()

private:

	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	UPROPERTY(EditAnywhere, Category = SpaceInvader3D)
	TSubclassOf<class USpaceInvader3DOverlay> SpaceInvader3DOverlayClass;

	UPROPERTY()
	TObjectPtr<class USpaceInvader3DOverlay> SpaceInvader3DOverlay;

public:

	FORCEINLINE TObjectPtr<USpaceInvader3DOverlay> GetSpaceInvader3DOverlay() const { return SpaceInvader3DOverlay; }
};
