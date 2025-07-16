#include "Enemies/EnemyShipSpawnPoint.h"
#include "Components/SphereComponent.h"

AEnemyShipSpawnPoint::AEnemyShipSpawnPoint() {
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
}

void AEnemyShipSpawnPoint::BeginPlay() {
	Super::BeginPlay();
}

void AEnemyShipSpawnPoint::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
