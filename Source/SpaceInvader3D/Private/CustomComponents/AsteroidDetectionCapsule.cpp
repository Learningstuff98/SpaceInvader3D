// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomComponents/AsteroidDetectionCapsule.h"
#include "Asteroids/Asteroid.h"

UAsteroidDetectionCapsule::UAsteroidDetectionCapsule() {
	bIsFlyingDirectlyTowardsAnAsteroid = false;
}

void UAsteroidDetectionCapsule::BeginPlay() {
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UAsteroidDetectionCapsule::OnCapsuleOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UAsteroidDetectionCapsule::OnCapsuleEndOverlap);
}

void UAsteroidDetectionCapsule::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		if (TObjectPtr<AAsteroid> Asteroid = Cast<AAsteroid>(OtherActor)) {
			bIsFlyingDirectlyTowardsAnAsteroid = true;
		}
	}
}

void UAsteroidDetectionCapsule::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	bIsFlyingDirectlyTowardsAnAsteroid = false;
}

bool UAsteroidDetectionCapsule::GetbIsFlyingDirectlyTowardsAnAsteroid() {
	return bIsFlyingDirectlyTowardsAnAsteroid;
}
