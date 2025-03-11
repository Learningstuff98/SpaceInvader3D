// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/PlayerShipAttributes.h"

UPlayerShipAttributes::UPlayerShipAttributes() {
	PrimaryComponentTick.bCanEverTick = true;

	Health = 500.0f;
	MaxHealth = 500.0f;
}

void UPlayerShipAttributes::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerShipAttributes::SetCurrentVelocity(const FVector& MovementComponentVelocity) {
	CurrentVelocity = MovementComponentVelocity;
}

void UPlayerShipAttributes::ApplyCollisionDamage() {
	Health -= 100.0f; // damage should be based on current velocity. also health sould be a float 
}

float UPlayerShipAttributes::GetHealthPercent() {
	return Health / MaxHealth;
}

void UPlayerShipAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
