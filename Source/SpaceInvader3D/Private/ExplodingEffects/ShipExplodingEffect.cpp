#include "ExplodingEffects/ShipExplodingEffect.h"
#include "NiagaraComponent.h"

AShipExplodingEffect::AShipExplodingEffect() {
	PrimaryActorTick.bCanEverTick = true;

	ExplosionEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion Effect"));
	SetRootComponent(ExplosionEffect);
}

void AShipExplodingEffect::BeginPlay() {
	Super::BeginPlay();
}

void AShipExplodingEffect::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
