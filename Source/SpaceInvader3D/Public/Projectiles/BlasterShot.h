#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlasterShot.generated.h"

UCLASS()
class SPACEINVADER3D_API ABlasterShot : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlasterShot();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

	// Functions

	void FireInDirection(const FVector& ShootDirection);
	void SpawnImpactBurst();

	// Variables

	UPROPERTY()
	int32 Damage;

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> BlasterShotSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> BlasterShotEffect;

	// Varibles

	UPROPERTY(EditAnywhere, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> ImpactBurst;
};
