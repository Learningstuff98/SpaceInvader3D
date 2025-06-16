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

	// Functions
	void Move();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> MissileThusterEffect;

	// Other

	UPROPERTY()
	FVector MovementDirection;

	UPROPERTY()
	float Speed;

public:
	// Setters and getters
	FORCEINLINE void SetDirection(const FVector& Direction) { MovementDirection = Direction; }
};
