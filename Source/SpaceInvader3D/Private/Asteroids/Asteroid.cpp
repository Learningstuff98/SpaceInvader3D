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

	AsteroidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid Mesh"));
	SetRootComponent(AsteroidMeshComponent);
	AsteroidMeshComponent->SetNotifyRigidBodyCollision(true);
	AsteroidMeshComponent->SetCollisionProfileName(FName("Custom"));
	AsteroidMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AsteroidMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AsteroidMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void AAsteroid::BeginPlay() {
	Super::BeginPlay();
	AsteroidMeshComponent->OnComponentHit.AddDynamic(this, &AAsteroid::OnMeshHit);
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Rotate();
}

void AAsteroid::Rotate() {
	FRotator OldRotation = GetActorRotation();
	const FRotator NewRotation(
		OldRotation.Pitch,
		OldRotation.Roll,
		OldRotation.Yaw + 0.1
	);
	SetActorRotation(NewRotation);
}

void AAsteroid::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor) {
		if (const TObjectPtr<APlayerShip> PlayerShip = Cast<APlayerShip>(OtherActor)) {
			HandlePlayerShipImpact(PlayerShip);
		}
		if (const TObjectPtr<ABlasterShot> BlasterShot = Cast<ABlasterShot>(OtherActor)) {
			HandleBlasterShotImpact(BlasterShot);
		}
	}
}

void AAsteroid::HandleBlasterShotImpact(const TObjectPtr<ABlasterShot> BlasterShot) {
	BlasterShot->SpawnImpactBurst();
	BlasterShot->Destroy();
	PlayImpactSound(BlasterShotImpactSound);
}

void AAsteroid::HandlePlayerShipImpact(const TObjectPtr<APlayerShip> PlayerShip) {
	if (PlayerShip->PlayerShipAttributes) {
		PlayerShip->PlayerShipAttributes->ApplyCollisionDamage();
	}
}

void AAsteroid::PlayImpactSound(const TObjectPtr<USoundBase> ImpactSound) {
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		GetActorLocation()
	);
}
