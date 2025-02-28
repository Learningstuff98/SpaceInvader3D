// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroids/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

AAsteroid::AAsteroid() {
	PrimaryActorTick.bCanEverTick = true;

	AsteroidSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Asteroid Sphere"));
	SetRootComponent(AsteroidSphere);
	AsteroidSphere->SetCollisionProfileName(FName("OverlapAllDynamic"));

	AsteroidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid Mesh"));
	AsteroidMeshComponent->SetupAttachment(GetRootComponent());
}

void AAsteroid::BeginPlay() {
	Super::BeginPlay();
	AsteroidSphere->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnSphereOverlap);
}

void AAsteroid::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	LogMessage("THE ASTEROID WAS HIT");
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAsteroid::LogMessage(const FString& Message) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}
