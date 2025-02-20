#include "Projectiles/BlasterShot.h"
#include "Components/CapsuleComponent.h"

ABlasterShot::ABlasterShot() {
	PrimaryActorTick.bCanEverTick = true;

	BlasterShotCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Blaster Shot Capsule"));
	SetRootComponent(BlasterShotCapsule);
	BlasterShotCapsule->SetCapsuleHalfHeight(50.f);
	BlasterShotCapsule->SetCapsuleRadius(17.f);
}

void ABlasterShot::BeginPlay() {
	Super::BeginPlay();
}

void ABlasterShot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
