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

	// Miscellaneous
	void SetupMappingContext();
	void SetThrusterPitch();
	void SetThrusterColor();
	void PlayBlasterSound();
	void Fire();
	TObjectPtr<class ABlasterShot> SpawnBlasterShot();
	TObjectPtr<class UArrowComponent> DeterminWhichBarrelToFireFrom();
	void SetInitialSpeed();
	void UpdateSpeed();
	TObjectPtr<class USpaceInvader3DOverlay> SetOverlay();
	void SetHealthBarPercent();
	void PlayExplodingSound();
	void HandleExplodingSound();

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

	// Other Variables

	UPROPERTY()
	int32 BarrelNumberToFireFrom;

	UPROPERTY()
	float MaxSpeed;

	UPROPERTY()
	float MinSpeed;

	FTimerHandle FireCooldownTimer;

	UPROPERTY()
	bool bFireCooldownTimerFinished;

	UPROPERTY(EditAnywhere, Category = Blaster)
	TObjectPtr<class USoundBase> BlasterSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<class USoundBase> ExplodingSound;

	UPROPERTY()
	bool bHasPlayedExplodingSound;

	UPROPERTY(EditAnywhere, Category = Blaster)
	TSubclassOf<class ABlasterShot> BlasterShotBlueprintClass;

	UPROPERTY()
	TObjectPtr<class USpaceInvader3DOverlay> PlayerShipOverlay;
};
