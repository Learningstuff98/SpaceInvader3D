#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceInvaderGameState.generated.h"

UCLASS()
class SPACEINVADER3D_API ASpaceInvaderGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	int32 EnemyShipCount;

public:
	// Setters and getters
	FORCEINLINE void SetEnemyShipCount(const int32& NewCount) { EnemyShipCount = NewCount; }
	FORCEINLINE int32 GetEnemyShipCount() const { return EnemyShipCount; }
};
