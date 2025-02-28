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
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Development
	void LogMessage(const FString& Message);

	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> AsteroidSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> AsteroidMeshComponent;
};
