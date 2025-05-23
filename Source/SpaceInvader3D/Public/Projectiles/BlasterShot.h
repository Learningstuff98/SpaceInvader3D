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

	// Miscellaneous
	void FireInDirection(const FVector& ShootDirection);
	void SpawnImpactBurst();

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

	// Other Varibles

	UPROPERTY(EditAnywhere, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> ImpactBurst;
};
