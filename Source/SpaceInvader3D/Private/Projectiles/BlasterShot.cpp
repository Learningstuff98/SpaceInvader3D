#include "Projectiles/BlasterShot.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"

ABlasterShot::ABlasterShot() {
	PrimaryActorTick.bCanEverTick = true;

	BlasterShotSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Blaster Shot Sphere"));
	SetRootComponent(BlasterShotSphere);
	BlasterShotSphere->SetSphereRadius(22.f);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;
	Movement->InitialSpeed = 90000.0f;

	BlasterShotEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Blaster Shot Effect"));
	BlasterShotEffect->SetupAttachment(GetRootComponent());
}

void ABlasterShot::BeginPlay() {
	Super::BeginPlay();
}

void ABlasterShot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ABlasterShot::FireInDirection(const FVector& ShootDirection) {
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
}
