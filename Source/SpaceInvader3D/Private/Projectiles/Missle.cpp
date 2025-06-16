#include "Projectiles/Missle.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

AMissle::AMissle() {
	PrimaryActorTick.bCanEverTick = true;
	Speed = 300.f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	MissileThusterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Missile Thuster Effect"));
	MissileThusterEffect->SetupAttachment(GetRootComponent());
}

void AMissle::BeginPlay() {
	Super::BeginPlay();
}

void AMissle::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	Move();
}

void AMissle::Move() {
	AddActorWorldOffset(MovementDirection * Speed);
}
