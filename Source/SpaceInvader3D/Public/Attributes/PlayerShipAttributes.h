// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerShipAttributes.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEINVADER3D_API UPlayerShipAttributes : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerShipAttributes();

	// <UActorComponent>
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// </UActorComponent>

private:
	// <UActorComponent>
	virtual void BeginPlay() override;
	// </UActorComponent>

	// Variables

	UPROPERTY(EditAnywhere, Category = Health)
	float Health;

	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth;

	UPROPERTY()
	FVector CurrentVelocity;

public:
	// Public Setters and getters
	void SetCurrentVelocity(const FVector& MovementComponentVelocity);
	void ApplyCollisionDamage();
	float GetHealthPercent();
};
