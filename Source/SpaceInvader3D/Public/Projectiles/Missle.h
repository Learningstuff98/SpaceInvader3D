#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missle.generated.h"

UCLASS()
class SPACEINVADER3D_API AMissle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissle();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:

	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Delegate Callbacks

	UFUNCTION()
	void ExplodeOnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Functions
	void Move();
	void UpdateRotation();
	FRotator FindTargetLookAtRotation();
	void SpawnExplodingEffect();
	void PlayFiringSound();
	void SetupOverlappingFunctionality();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> MissileThusterEffect;

	// Other

	UPROPERTY()
	float Speed;

	UPROPERTY()
	TObjectPtr<class AEnemyShip> Target;

	UPROPERTY()
	float RotationSpeed;

	UPROPERTY()
	int32 Damage;

	UPROPERTY(EditAnywhere, Category = Explosion)
	TSubclassOf<class AMissileExplodingEffect> MissileExplodingEffectBlueprintClass;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> FiringSound;

public:

	// Setters and getters
	FORCEINLINE void SetTarget(const TObjectPtr<class AEnemyShip> EnemyShip) { Target = EnemyShip; }
};
