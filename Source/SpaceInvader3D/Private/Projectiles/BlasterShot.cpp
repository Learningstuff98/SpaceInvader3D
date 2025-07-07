#include "Projectiles/BlasterShot.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemies/EnemyShip.h"
#include "Statics/ShipStatics.h"
#include "Asteroids/Asteroid.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip/PlayerShip.h"
#include "Attributes/PlayerShipAttributes.h"
#include "Components/StaticMeshComponent.h"

ABlasterShot::ABlasterShot() {
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 2.f;
	Damage = 100;

	BlasterShotSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Blaster Shot Sphere"));
	SetRootComponent(BlasterShotSphere);
	BlasterShotSphere->SetSphereRadius(15.f);
	BlasterShotSphere->SetNotifyRigidBodyCollision(true);
	BlasterShotSphere->SetCollisionProfileName(FName("Custom"));
	BlasterShotSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlasterShotSphere->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	BlasterShotSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BlasterShotSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BlasterShotSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;
	Movement->InitialSpeed = 200000.0f;

	BlasterShotVisualEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blaster Shot Visual Effect"));
	BlasterShotVisualEffect->SetupAttachment(GetRootComponent());
	BlasterShotVisualEffect->SetCollisionProfileName(FName("Custom"));
	BlasterShotVisualEffect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlasterShotVisualEffect->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

void ABlasterShot::BeginPlay() {
	Super::BeginPlay();
	PlaySound(FiringSound);
	SetupHitFunctionality();
}

void ABlasterShot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABlasterShot::SetupHitFunctionality() {
	if (BlasterShotSphere) BlasterShotSphere->OnComponentHit.AddDynamic(this, &ABlasterShot::DeliverHit);
}

void ABlasterShot::DeliverHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor) {
		if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
			HitEnemyShip(EnemyShip);
		}
		if (const TObjectPtr<APlayerShip> PlayerShip = Cast<APlayerShip>(OtherActor)) {
			if (this) {
				SpawnImpactBurst();
				PlayerShip->PlayerShipAttributes->TakeDamage(Damage);
				PlaySound(ImpactSound);
			}
			Destroy();
		}
		if (const TObjectPtr<AAsteroid> Asteroid = Cast<AAsteroid>(OtherActor)) {
			HitAsteroid();
		}
	}
}

void ABlasterShot::HitEnemyShip(const TObjectPtr<AEnemyShip> EnemyShip) {
	if (this) {
		SpawnImpactBurst();
		EnemyShip->TakeDamage(Damage);
		PlaySound(ImpactSound);
	}
	Destroy();
}

void ABlasterShot::HitAsteroid() {
	if (this) {
		SpawnImpactBurst();
		PlaySound(ImpactSound);
	}
	Destroy();
}

void ABlasterShot::SpawnImpactBurst() {
	if (ImpactBurst) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactBurst,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void ABlasterShot::PlaySound(const TObjectPtr<USoundBase> Sound) {
	if (Sound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound,
			GetActorLocation()
		);
	}
}

void ABlasterShot::FireInDirection(const FVector& ShootDirection) {
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
}
