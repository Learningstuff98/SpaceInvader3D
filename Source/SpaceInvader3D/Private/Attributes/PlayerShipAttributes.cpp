#include "Attributes/PlayerShipAttributes.h"

UPlayerShipAttributes::UPlayerShipAttributes() {
	PrimaryComponentTick.bCanEverTick = true;
	IsDead = false;
	Health = 2000.0f;
	MaxHealth = 2000.0f;
}

void UPlayerShipAttributes::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerShipAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerShipAttributes::ApplyCollisionDamage() {
	Health = 0.f;
	IsDead = true;
}

void UPlayerShipAttributes::TakeDamage(const int32& Damage) {
	Health -= Damage;
	if (Health <= 0) IsDead = true;
}

float UPlayerShipAttributes::GetHealthPercent() {
	return Health / MaxHealth;
}
