#include "Projectiles/BlasterShot.h"

ABlasterShot::ABlasterShot() {
	PrimaryActorTick.bCanEverTick = true;
}

void ABlasterShot::BeginPlay() {
	Super::BeginPlay();
}

void ABlasterShot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
