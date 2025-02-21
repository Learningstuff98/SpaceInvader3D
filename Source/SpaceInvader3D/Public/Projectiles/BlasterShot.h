// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlasterShot.generated.h"

UCLASS()
class SPACEINVADER3D_API ABlasterShot : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlasterShot();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> BlasterShotSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> BlasterShotEffect;
};
