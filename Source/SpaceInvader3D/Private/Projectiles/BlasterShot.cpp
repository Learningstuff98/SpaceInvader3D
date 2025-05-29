#include "Projectiles/BlasterShot.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

	BlasterShotEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Blaster Shot Effect"));
	BlasterShotEffect->SetupAttachment(GetRootComponent());
}

void ABlasterShot::BeginPlay() {
	Super::BeginPlay();
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

void ABlasterShot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABlasterShot::FireInDirection(const FVector& ShootDirection) {
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
}
