#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerShip.generated.h"

UCLASS()
class SPACEINVADER3D_API APlayerShip : public APawn
{
	GENERATED_BODY()

public:
	APlayerShip();

	// <AActor>
	virtual void Tick(float DeltaTime) override;
	// </AActor>

	// <APawn>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// </APawn>

	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerShipAttributes> PlayerShipAttributes;

private:
	// <APawn>
	virtual void BeginPlay() override;
	// </APawn>

	// Callbacks for input

	void Look(const FInputActionValue& Value);
	void Accelerate();
	void Decelerate();
	void ToggleViewMode();
	void HandleFireTimer();
	void RollLeft();
	void RollRight();
	void ToggleHeadlights();
	void SetTargetedEnemyShip();
	void FireMissle();

	// Delegate Callbacks

	UFUNCTION()
	void DetectEnemyShip(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void LoseEnemyShip(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void HandleLockingOntoEnemyShips(APawn* SeenPawn);

	// Functions

	void SetupMappingContext();
	void SetThrusterPitch();
	void SetThrusterColor();
	void Fire();
	TObjectPtr<class ABlasterShot> SpawnBlasterShot();
	TObjectPtr<class UArrowComponent> DeterminWhichBarrelToFireFrom();
	TObjectPtr<class USpaceInvader3DOverlay> SetOverlay();
	void SetHealthBarPercent();
	void HandleExploding();
	void Explode();
	void UpdatePlayerShipRotation(const float& DeltaTime);
	void SetCurrentControlSpeed(const double& ControlSpeedInput, const double& DeadZone, const double& MaxTurnSpeed, const double& Sensitivity, double& CurrentControlSpeed);
	void View(const FVector2D& LookAxisValue, const double& Sensitivity);
	void ResetControlRotation();
	void Steer(const FVector2D& LookAxisValue);
	void SetMovementComponentMaxSpeed();
	void DeactivateComponentsAfterExploding();
	void ZeroOutCurrentControlSpeed();
	void HandleHeadLightHUDText();
	bool HeadLightsAreOn();
	void TurnHeadLightsOn();
	void TurnHeadLightsOff();
	void SetupEnemyShipDetectionFunctionality();
	void ExitViewModeAfterExploding();
	void UpdateEnemyShipDirectionArrowRotation();
	void HandleEnemyShipDirectionArrowVisibility();
	void HandleTargetedEnemyShipStatus();
	void HandleLockedOnEnemyShipStatus();
	void HandleLockOnBeepSound();
	void SetupPawnSensing();
	void HandleLockedOnEnemyShipNullOutTimer();
	void NullOutLockedOnEnemyShip();
	bool CanFireMissile();
	void SetMissileReloadTimer();
	void ReloadMissile();
	void SetMissileReloadingProgressBar();
	float GetRemainingMissileReloadTimeAsPercent();
	 
	// Component Variables

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> ShipMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFloatingPawnMovement> Movement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> EngineThrusterEffect1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> EngineThrusterEffect2;

	UPROPERTY(VisibleAnywhere, Category = Sound)
	TObjectPtr<class UAudioComponent> CruisingThrusterSound;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> CameraResetTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> GunBarrel1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> GunBarrel2;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> GunBarrel3;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> GunBarrel4;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> OuterSightSpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> OuterSight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> InnerSightSpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> InnerSight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> SkyBox;

	UPROPERTY(visibleAnywhere)
	TObjectPtr<class USpotLightComponent> RightHeadLight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpotLightComponent> LeftHeadLight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> EnemyShipDetectionSphere;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> FieldSystemSpawnLocation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> EnemyShipDirectionArrow;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAudioComponent> LockedOnBeepingSound;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> MissleSpawnLocation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;

	// Enhanced Input Varaibles

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AccelerateAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DecelerateAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ToggleViewModeAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ZoomCameraAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> RollLeftAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> RollRightAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ToggleHeadlightsAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SetTargetedEnemyShipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> FireMissleAction;

	// Other

	UPROPERTY()
	int32 BarrelNumberToFireFrom;

	UPROPERTY()
	float MaxSpeed;

	UPROPERTY()
	float MinSpeed;

	FTimerHandle FireCooldownTimer;

	UPROPERTY()
	bool FireCooldownTimerFinished;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> BlasterSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> ExplodingSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> ToggleHeadLightSound;

	UPROPERTY(EditAnywhere, Category = Projectiles)
	TSubclassOf<class ABlasterShot> BlasterShotBlueprintClass;

	UPROPERTY(EditAnywhere, Category = Projectiles)
	TSubclassOf<class AMissle> MissleBlueprintClass;

	UPROPERTY(EditAnywhere, Category = Pieces)
	TSubclassOf<class AShipPieces> ShipPiecesBlueprintClass;

	UPROPERTY(EditAnywhere, Category = "Field Systems")
	TSubclassOf<class AFieldSystemActor> ShipExplodingFieldSystemBlueprintClass;

	UPROPERTY(EditAnywhere, Category = "Explosion Effect")
	TSubclassOf<class AShipExplodingEffect> ShipExplodingEffectBlueprintClass;

	UPROPERTY()
	TObjectPtr<class USpaceInvader3DOverlay> PlayerShipOverlay;

	UPROPERTY()
	bool HasHandledExploding;

	UPROPERTY()
	double CurrentPitchControlSpeed;

	UPROPERTY()
	double CurrentYawControlSpeed;

	UPROPERTY()
	bool InViewMode;

	UPROPERTY()
	float CurrentSpeed;

	UPROPERTY()
	float MissileReloadTime;

	FTimerHandle MissileReloadTimer;

	UPROPERTY()
	bool MissileReloadTimerFinished;

	UPROPERTY()
	bool MissileIsLoaded;

	FTimerHandle LockedOnEnemyShipNullOutTimer;

	UPROPERTY()
	bool LockedOnEnemyShipNullOutTimerFinished;

	UPROPERTY(EditAnywhere, Category = Enemies)
	TObjectPtr<class AEnemyShip> TargetedEnemyShip;

	UPROPERTY(EditAnywhere, Category = Enemies)
	TArray<TObjectPtr<class AEnemyShip>> DetectedEnemyShips;

	UPROPERTY(EditAnywhere, Category = Enemies)
	TObjectPtr<class AEnemyShip> LockedOnEnemyShip;

public:
    // Getters and setters
	FVector GetCameraLocation();
};
