#include "ExplodingEffects/MissileExplodingEffect.h"
#include "NiagaraComponent.h"

AMissileExplodingEffect::AMissileExplodingEffect() {
	PrimaryActorTick.bCanEverTick = true;

	ExplosionEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ExplosionEffect"));
	SetRootComponent(ExplosionEffect);
}

void AMissileExplodingEffect::BeginPlay() {
	Super::BeginPlay();
}

void AMissileExplodingEffect::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
