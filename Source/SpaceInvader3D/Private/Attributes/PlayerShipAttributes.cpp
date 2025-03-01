// Fill out your copyright notice in the Description page of Project Settings.

#include "Attributes/PlayerShipAttributes.h"

UPlayerShipAttributes::UPlayerShipAttributes() {
	PrimaryComponentTick.bCanEverTick = true;

	Health = 500.0f;
}

void UPlayerShipAttributes::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerShipAttributes::SetHealthAfterAsteroidImpact() {
	Health -= abs(CurrentVelocity.X) * 0.003f;
}

void UPlayerShipAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
