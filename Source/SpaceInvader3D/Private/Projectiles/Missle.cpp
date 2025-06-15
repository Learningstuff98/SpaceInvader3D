#include "Projectiles/Missle.h"

AMissle::AMissle() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMissle::BeginPlay() {
	Super::BeginPlay();
}

void AMissle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
