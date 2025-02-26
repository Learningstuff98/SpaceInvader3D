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
	SetRandomMesh();
}

void AAsteroid::SetRandomMesh() {
	if (AsteroidMeshComponent) {
		AsteroidMeshComponent->SetStaticMesh(AsteroidMeshes[GetRandomIndex()]);
	}
}

int32 AAsteroid::GetRandomIndex() {
	return FMath::RandRange(0, 2);
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
