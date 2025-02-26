// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroids/Asteroid.h"

AAsteroid::AAsteroid() {
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroid::BeginPlay() {
	Super::BeginPlay();
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
