#include "Projectiles/Missle.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemies/EnemyShip.h"
#include "Kismet/GameplayStatics.h"

AMissle::AMissle() {
	PrimaryActorTick.bCanEverTick = true;
	Speed = 20000.f;
	RotationSpeed = 1.f;
	InitialLifeSpan = 5.f;
	Target = nullptr;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	MissileThusterEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Missile Thuster Effect"));
	MissileThusterEffect->SetupAttachment(GetRootComponent());
}

void AMissle::BeginPlay() {
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMissle::ExplodeOnHit);
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
		UKismetMathLibrary::RInterpTo(
			GetActorRotation(),
			FindTargetLookAtRotation(),
			UGameplayStatics::GetWorldDeltaSeconds(this),
			RotationSpeed
		)
	);
}

FRotator AMissle::FindTargetLookAtRotation() {
	return UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(),
		Target->GetActorLocation()
	);
}

void AMissle::ExplodeOnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
		Destroy();
	}
}
