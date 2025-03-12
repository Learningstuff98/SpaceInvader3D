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

	// Functions
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PlayImpactSound();
	void HandlePlayerShipImpact(const TObjectPtr<class APlayerShip> PlayerShip);

	// Development
	void LogMessage(const FString& Message);

	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> AsteroidSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> AsteroidDetectionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> AsteroidMeshComponent;

	// Other Variables

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> ImpactSound;

	UPROPERTY()
	bool bHasPerformedImpact;
};
