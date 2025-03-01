// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroids/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerShip/PlayerShip.h"
#include "Attributes/PlayerShipAttributes.h"

AAsteroid::AAsteroid() {
	PrimaryActorTick.bCanEverTick = true;

	AsteroidSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Asteroid Sphere"));
	SetRootComponent(AsteroidSphere);
	AsteroidSphere->SetNotifyRigidBodyCollision(true);
	AsteroidSphere->SetCollisionProfileName(FName("Custom"));
    AsteroidSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AsteroidSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AsteroidSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	AsteroidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid Mesh"));
	AsteroidMeshComponent->SetupAttachment(GetRootComponent());
}

void AAsteroid::BeginPlay() {
	Super::BeginPlay();
	AsteroidSphere->OnComponentHit.AddDynamic(this, &AAsteroid::OnSphereHit);
}

void AAsteroid::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	LogMessage("THE ASTEROID WAS HIT");
	if (OtherActor) {
		if (TObjectPtr<APlayerShip> PlayerShip = Cast<APlayerShip>(OtherActor)) {
			PlayerShip->PlayerShipAttributes->SetHealth();
		}
	}
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAsteroid::LogMessage(const FString& Message) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}
