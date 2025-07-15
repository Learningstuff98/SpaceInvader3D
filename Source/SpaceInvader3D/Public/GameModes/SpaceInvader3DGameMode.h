#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvader3DGameMode.generated.h"

UCLASS()
class SPACEINVADER3D_API ASpaceInvader3DGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceInvader3DGameMode();

	//<AActor>
	virtual void Tick(float DeltaTime) override;
	//</AActor>

private:
	// <AActor>
	virtual void BeginPlay() override;
	// </AActor>

	// Delegate callbacks

	UFUNCTION()
	void HandleDetectionSphereOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Functions

	void HandleDetectionSphere();
	void PerformDetectionSpherePing();
	void HandleDiscardingDetectionSphere();
	void SetupDetectionSphereOverlapFunctionality();

	// Components

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> DetectionSphere;

	// Other

	UPROPERTY(EditAnywhere, Category = "Patrol Targets")
	TArray<TObjectPtr<class APatrolTarget>> PatrolTargets;
};
