// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "AsteroidDetectionCapsule.generated.h"

UCLASS()
class SPACEINVADER3D_API UAsteroidDetectionCapsule : public UCapsuleComponent
{
	GENERATED_BODY()
	
public:
	UAsteroidDetectionCapsule();

	// <UPrimitiveComponent>
	virtual void BeginPlay() override;
	// </UPrimitiveComponent>

private:

	UPROPERTY(EditAnywhere)
	bool bIsFlyingDirectlyTowardsAnAsteroid;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	bool GetbIsFlyingDirectlyTowardsAnAsteroid();
};
