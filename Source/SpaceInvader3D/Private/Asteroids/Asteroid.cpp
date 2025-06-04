#include "Asteroids/Asteroid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerShip/PlayerShip.h"
#include "Attributes/PlayerShipAttributes.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/BlasterShot.h"

AAsteroid::AAsteroid() {
	PrimaryActorTick.bCanEverTick = true;
	bHasPositiveRotation = false;
	RotationalDrift = 0.0;

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
	SetupCollisionFunctionality();
}

void AAsteroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Rotate(DeltaTime);
}

void AAsteroid::SetupCollisionFunctionality() {
	if (AsteroidMeshComponent) {
		AsteroidMeshComponent->OnComponentHit.AddDynamic(this, &AAsteroid::OnMeshHit);
	}
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
	PlayImpactSound();
}

void AAsteroid::HandlePlayerShipImpact(const TObjectPtr<APlayerShip> PlayerShip) {
	if (PlayerShip->PlayerShipAttributes) {
		PlayerShip->PlayerShipAttributes->ApplyCollisionDamage();
	}
}

void AAsteroid::PlayImpactSound() {
	UGameplayStatics::PlaySoundAtLocation(
		this,
		BlasterShotImpactSound,
		GetActorLocation()
	);
}

void AAsteroid::Rotate(const float& DeltaTime) {
	const FRotator OldRotation = GetActorRotation();
	SetActorRotation(
	    FRotator(
			OldRotation.Pitch,
			GetNewRotationRoll(OldRotation, DeltaTime),
			OldRotation.Yaw
		)
	);
}

double AAsteroid::GetNewRotationRoll(const FRotator& OldRotation, const float& DeltaTime) {
	if (bHasPositiveRotation) {
		return OldRotation.Roll + (RotationalDrift * DeltaTime);
	}
	else {
		return OldRotation.Roll - (RotationalDrift * DeltaTime);
	}
}
