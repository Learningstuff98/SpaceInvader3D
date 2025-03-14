// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroids/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerShip/PlayerShip.h"
#include "Attributes/PlayerShipAttributes.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/BlasterShot.h"

AAsteroid::AAsteroid() {
	PrimaryActorTick.bCanEverTick = true;

	bHasPerformedImpact = false;

	AsteroidSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Asteroid Sphere"));
	SetRootComponent(AsteroidSphere);
	AsteroidSphere->SetNotifyRigidBodyCollision(true);
	AsteroidSphere->SetCollisionProfileName(FName("Custom"));
    AsteroidSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AsteroidSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AsteroidSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	AsteroidDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Asteroid Detection Sphere"));
	AsteroidDetectionSphere->SetupAttachment(GetRootComponent());

	AsteroidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid Mesh"));
	AsteroidMeshComponent->SetupAttachment(GetRootComponent());
}

void AAsteroid::BeginPlay() {
	Super::BeginPlay();
	AsteroidSphere->OnComponentHit.AddDynamic(this, &AAsteroid::OnSphereHit);
	AsteroidDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAsteroid::OnDetectionSphereEndOverlap);
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAsteroid::OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	bHasPerformedImpact = false;
}

void AAsteroid::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor) {
		if (const TObjectPtr<APlayerShip> PlayerShip = Cast<APlayerShip>(OtherActor)) {
			HandlePlayerShipImpact(PlayerShip);
		}
		if (const TObjectPtr<ABlasterShot> BlasterShot = Cast<ABlasterShot>(OtherActor)) {
			HandleBlasterShotImpact(BlasterShot);
		}
	}
}

void AAsteroid::HandleBlasterShotImpact(const TObjectPtr<class ABlasterShot> BlasterShot) {
	BlasterShot->Destroy();
	PlayImpactSound(BlasterShotImpactSound);
}

void AAsteroid::HandlePlayerShipImpact(const TObjectPtr<APlayerShip> PlayerShip) {
	if (PlayerShip->PlayerShipAttributes && !bHasPerformedImpact) {
		PlayerShip->PlayerShipAttributes->ApplyCollisionDamage();
		if (!PlayerShip->PlayerShipAttributes->GetbHasBlownUp()) {
			PlayImpactSound(ShipImpactSound);
		}
		bHasPerformedImpact = true;
	}
}

void AAsteroid::PlayImpactSound(const TObjectPtr<USoundBase> ImpactSound) {
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		GetActorLocation()
	);
}
