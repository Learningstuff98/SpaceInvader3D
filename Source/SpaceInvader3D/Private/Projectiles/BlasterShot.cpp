#include "Projectiles/BlasterShot.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemies/EnemyShip.h"
#include "Statics/ShipStatics.h"
#include "Asteroids/Asteroid.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip/PlayerShip.h"
#include "Attributes/PlayerShipAttributes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ABlasterShot::ABlasterShot() {
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.5f;
	Damage = 100;

	BlasterShotVisualEffect = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blaster Shot Visual Effect"));
	SetRootComponent(BlasterShotVisualEffect);
	BlasterShotVisualEffect->SetNotifyRigidBodyCollision(true);
	BlasterShotVisualEffect->SetCollisionProfileName(FName("Custom"));
	BlasterShotVisualEffect->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlasterShotVisualEffect->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	BlasterShotVisualEffect->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BlasterShotVisualEffect->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BlasterShotVisualEffect->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;

	ImpactBurstSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Impact Burst Spawn Location"));
	ImpactBurstSpawnLocation->SetupAttachment(GetRootComponent());
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
	if (BlasterShotVisualEffect) {
		BlasterShotVisualEffect->OnComponentHit.AddDynamic(this, &ABlasterShot::DeliverHit);
	}
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
	if (ImpactBurst && ImpactBurstSpawnLocation) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactBurst,
			ImpactBurstSpawnLocation->GetComponentLocation(),
			ImpactBurstSpawnLocation->GetComponentRotation()
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

void ABlasterShot::SetMovementSpeed(const float& Speed) {
	if (Movement) Movement->InitialSpeed = Speed;
}

void ABlasterShot::FireInDirection(const FVector& ShootDirection) {
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
}
