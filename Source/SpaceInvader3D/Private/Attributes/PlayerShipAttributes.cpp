// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/PlayerShipAttributes.h"

UPlayerShipAttributes::UPlayerShipAttributes() {
	PrimaryComponentTick.bCanEverTick = true;

	Health = 500.0f;
	MaxHealth = 500.0f;
	bHasPlayedCrashSound = false;
}

void UPlayerShipAttributes::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerShipAttributes::SetCurrentVelocity(const FVector& MovementComponentVelocity) {
	CurrentVelocity = MovementComponentVelocity;
}

void UPlayerShipAttributes::ApplyHeadOnCollisionAsteroidDamage() {
	Health = 0;
}

float UPlayerShipAttributes::GetHealthPercent() {
	return Health / MaxHealth;
}

void UPlayerShipAttributes::SetbHasPlayedCrashSound(const bool& BoolValue) {
	bHasPlayedCrashSound = BoolValue;
}

void UPlayerShipAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
