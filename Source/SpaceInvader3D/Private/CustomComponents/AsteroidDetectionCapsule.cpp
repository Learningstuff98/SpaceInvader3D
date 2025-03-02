// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomComponents/AsteroidDetectionCapsule.h"
#include "CustomComponents/AsteroidCenterSphere.h"

UAsteroidDetectionCapsule::UAsteroidDetectionCapsule() {
	bIsFlyingTowardsTheCenterOfAnAsteroid = false;
}

void UAsteroidDetectionCapsule::BeginPlay() {
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UAsteroidDetectionCapsule::OnSphereOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UAsteroidDetectionCapsule::OnSphereEndOverlap);
}

void UAsteroidDetectionCapsule::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherComp) {
		if (TObjectPtr<UAsteroidCenterSphere> AsteroidCenterSphere = Cast<UAsteroidCenterSphere>(OtherComp)) {
			bIsFlyingTowardsTheCenterOfAnAsteroid = true;
		}
	}
}

void UAsteroidDetectionCapsule::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	bIsFlyingTowardsTheCenterOfAnAsteroid = false;
}
