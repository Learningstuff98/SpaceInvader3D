#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolTarget.generated.h"

UCLASS()
class SPACEINVADER3D_API APatrolTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolTarget();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Delegate Callbacks

	UFUNCTION()
	void TellEnemyShipToFindANewPatrolTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Functions

	void SetupSphereInteractionsFunctionality();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> EnemyShipDetectionSphere;
};
