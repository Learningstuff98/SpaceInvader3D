#include "Projectiles/Missle.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"

AMissle::AMissle() {
	PrimaryActorTick.bCanEverTick = true;
	Speed = 13000.f;
	Target = nullptr;

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
	UpdateRotation();
}

void AMissle::Move() {
	AddActorWorldOffset(
		GetActorForwardVector() * Speed * UGameplayStatics::GetWorldDeltaSeconds(this)
	);
}

void AMissle::UpdateRotation() {
	SetActorRotation(
	    UKismetMathLibrary::FindLookAtRotation(
		    GetActorLocation(),
			Target->GetActorLocation()
		)
	);
}
