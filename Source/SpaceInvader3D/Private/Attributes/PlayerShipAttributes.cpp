// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/PlayerShipAttributes.h"

UPlayerShipAttributes::UPlayerShipAttributes() {
	PrimaryComponentTick.bCanEverTick = true;
	bHasBlownUp = false;
	Health = 500.0f;
	MaxHealth = 500.0f;
}

void UPlayerShipAttributes::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerShipAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerShipAttributes::ApplyCollisionDamage() {
	Health = 0.f;
	HandleHasBlownUpStatus();
}

void UPlayerShipAttributes::HandleHasBlownUpStatus() {
	if (Health <= 0.f) {
		bHasBlownUp = true;
	}
}

float UPlayerShipAttributes::GetHealthPercent() {
	return Health / MaxHealth;
}
