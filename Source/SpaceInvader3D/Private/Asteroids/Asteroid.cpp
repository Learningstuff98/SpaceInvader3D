// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroids/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

AAsteroid::AAsteroid() {
	PrimaryActorTick.bCanEverTick = true;

	AsteroidSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Asteroid Sphere"));
	SetRootComponent(AsteroidSphere);

	AsteroidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid Mesh"));
	AsteroidMeshComponent->SetupAttachment(GetRootComponent());
}

void AAsteroid::BeginPlay() {
	Super::BeginPlay();
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
