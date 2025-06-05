// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

UCLASS()
class SPACEINVADER3D_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	AAsteroid();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Delegate Callbacks

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Functions

	void PlayImpactSound();
	void HandlePlayerShipImpact(const TObjectPtr<class APlayerShip> PlayerShip);
	void HandleBlasterShotImpact(const TObjectPtr<class ABlasterShot> BlasterShot);
	void Rotate(const float& DeltaTime);
	double GetNewRotationRoll(const FRotator& OldRotation, const float& DeltaTime);
	void SetupCollisionFunctionality();

	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> AsteroidSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> AsteroidMeshComponent;

	// Other Variables

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> BlasterShotImpactSound;

	UPROPERTY(EditAnywhere, Category = "Rotational Drift")
	double RotationalDrift;

	UPROPERTY(EditAnywhere, Category = "Rotational Drift")
	bool HasPositiveRotation;
};
