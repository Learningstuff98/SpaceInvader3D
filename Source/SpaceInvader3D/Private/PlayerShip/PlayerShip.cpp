#include "PlayerShip/PlayerShip.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/BlasterShot.h"
#include "Attributes/PlayerShipAttributes.h"
#include "HUD/SpaceInvader3DHUD.h"
#include "HUD/SpaceInvader3DOverlay.h"
#include "Engine/StaticMesh.h"
#include "Components/SpotLightComponent.h"
#include "Components/SphereComponent.h"
#include "Enemies/EnemyShip.h"
#include "Kismet/KismetMathLibrary.h"
#include "Statics/ShipStatics.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

APlayerShip::APlayerShip() {
	PrimaryActorTick.bCanEverTick = true;
	FireCooldownTimerFinished = true;
	HasHandledExploding = false;
	InViewMode = false;
	CurrentPitchControlSpeed = 0.0;
	CurrentYawControlSpeed = 0.0;
	BarrelNumberToFireFrom = 1;
	MaxSpeed = 12000.0f;
	MinSpeed = 3300.0f;
	CurrentSpeed = MinSpeed;
	TargetedEnemyShip = nullptr;
	PotentiallyLockedOnEnemyShip = nullptr;

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMeshComponent"));
	SetRootComponent(ShipMeshComponent);
	ShipMeshComponent->SetNotifyRigidBodyCollision(true);
	ShipMeshComponent->SetCollisionProfileName(FName("Custom"));
	ShipMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ShipMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ShipMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 1500.f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 450.0f);
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 2.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 6.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PlayerShipAttributes = CreateDefaultSubobject<UPlayerShipAttributes>(TEXT("Player Ship Attributes"));

	CameraResetTarget = CreateDefaultSubobject<UArrowComponent>(TEXT("Camera Reset Target"));
	CameraResetTarget->SetupAttachment(GetRootComponent());
	CameraResetTarget->SetRelativeLocation(FVector(-1600.0f, 0.0f, 450.0f));

	GunBarrel1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Gun Barrel 1"));
	GunBarrel1->SetupAttachment(GetRootComponent());

	GunBarrel2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Gun Barrel 2"));
	GunBarrel2->SetupAttachment(GetRootComponent());

	GunBarrel3 = CreateDefaultSubobject<UArrowComponent>(TEXT("Gun Barrel 3"));
	GunBarrel3->SetupAttachment(GetRootComponent());

	GunBarrel4 = CreateDefaultSubobject<UArrowComponent>(TEXT("Gun Barrel 4"));
	GunBarrel4->SetupAttachment(GetRootComponent());

	OuterSightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Outer Sight Spring Arm"));
	OuterSightSpringArm->SetupAttachment(GetRootComponent());

	OuterSight = CreateDefaultSubobject<UBoxComponent>(TEXT("Outer Sight"));
	OuterSight->SetupAttachment(OuterSightSpringArm);
	OuterSight->bHiddenInGame = false;
	OuterSight->SetCollisionProfileName(FName("Custom"));
	OuterSight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterSight->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	OuterSight->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OuterSight->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	InnerSightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Inner Sight Spring Arm"));
	InnerSightSpringArm->SetupAttachment(GetRootComponent());

	InnerSight = CreateDefaultSubobject<UBoxComponent>(TEXT("Inner Sight"));
	InnerSight->SetupAttachment(InnerSightSpringArm);
	InnerSight->bHiddenInGame = false;
	InnerSight->SetCollisionProfileName(FName("Custom"));
	InnerSight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerSight->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	InnerSight->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InnerSight->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));

	EngineThrusterEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 1"));
	EngineThrusterEffect1->SetupAttachment(GetRootComponent());

	EngineThrusterEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 2"));
	EngineThrusterEffect2->SetupAttachment(GetRootComponent());

	CruisingThrusterSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Cruising Thruster Sound"));
	CruisingThrusterSound->SetVolumeMultiplier(0.02f);

	SkyBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Skybox Mesh"));
	SkyBox->SetupAttachment(GetRootComponent());
	SkyBox->SetWorldScale3D(FVector(1000000.0, 1000000.0, 1000000.0));
	SkyBox->SetReverseCulling(true);

	RightHeadLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Right Head Light"));
	RightHeadLight->SetupAttachment(GetRootComponent());
	RightHeadLight->SetIntensityUnits(ELightUnits::EV);
	RightHeadLight->SetIntensity(17.f);
	RightHeadLight->SetAttenuationRadius(20000.f);
	RightHeadLight->SetOuterConeAngle(15.f);

	LeftHeadLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Left Head Light"));
	LeftHeadLight->SetupAttachment(GetRootComponent());
	LeftHeadLight->SetIntensityUnits(ELightUnits::EV);
	LeftHeadLight->SetIntensity(17.f);
	LeftHeadLight->SetAttenuationRadius(20000.f);
	LeftHeadLight->SetOuterConeAngle(15.f);

	EnemyShipDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Ship Detection Sphere"));
	EnemyShipDetectionSphere->SetupAttachment(GetRootComponent());
	EnemyShipDetectionSphere->SetSphereRadius(180000.0f);

	FieldSystemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Field System Spawn Location"));
	FieldSystemSpawnLocation->SetupAttachment(GetRootComponent());

	EnemyShipDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Enemy Ship Direction Arrow"));
	EnemyShipDirectionArrow->SetupAttachment(GetRootComponent());
	EnemyShipDirectionArrow->bHiddenInGame = false;
	EnemyShipDirectionArrow->SetArrowSize(4.0f);

	MissleLockOnDetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Missle Lock On Detection Capsule"));
	MissleLockOnDetectionCapsule->SetupAttachment(GetRootComponent());
}

void APlayerShip::BeginPlay() {
	Super::BeginPlay();
	SetupMappingContext();
	PlayerShipOverlay = SetOverlay();
	SetHealthBarPercent();
	SetupEnemyShipDetectionFunctionality();
	SetupEnemyShipLockOnFunctionality();
	TurnHeadLightsOff();
}

void APlayerShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AddMovementInput(GetActorForwardVector(), 1.0f);
	SetThrusterPitch();
	SetThrusterColor();
	SetHealthBarPercent();
	HandleExploding();
	UpdatePlayerShipRotation(DeltaTime);
	SetMovementComponentMaxSpeed();
	HandleHeadLightHUDText();
	HandleEnemyShipDirectionArrowVisibility();
	UpdateEnemyShipDirectionArrowRotation();
	HandleTargetedEnemyShipStatus();
}

void APlayerShip::SetupMappingContext() {
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController())) {
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerShip::Look);
		EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &APlayerShip::Accelerate);
		EnhancedInputComponent->BindAction(DecelerateAction, ETriggerEvent::Triggered, this, &APlayerShip::Decelerate);
		EnhancedInputComponent->BindAction(ToggleViewModeAction, ETriggerEvent::Triggered, this, &APlayerShip::ToggleViewMode);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerShip::HandleFireTimer);
		EnhancedInputComponent->BindAction(RollLeftAction, ETriggerEvent::Triggered, this, &APlayerShip::RollLeft);
		EnhancedInputComponent->BindAction(RollRightAction, ETriggerEvent::Triggered, this, &APlayerShip::RollRight);
		EnhancedInputComponent->BindAction(ToggleHeadlightsAction, ETriggerEvent::Triggered, this, &APlayerShip::ToggleHeadlights);
		EnhancedInputComponent->BindAction(SetTargetedEnemyShipAction, ETriggerEvent::Triggered, this, &APlayerShip::SetTargetedEnemyShip);
	}
}

void APlayerShip::HandleFireTimer() {
	if (!PlayerShipAttributes->GetIsDead()) {
		if (FireCooldownTimerFinished) {
			GetWorldTimerManager().ClearTimer(FireCooldownTimer);
			GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &APlayerShip::Fire, 0.15f);
			FireCooldownTimerFinished = false;
		}
    }
}

void APlayerShip::Fire() {
	if (const TObjectPtr<ABlasterShot> BlasterShot = SpawnBlasterShot()) {
		BlasterShot->FireInDirection(GetActorRotation().Vector());
	}
	FireCooldownTimerFinished = true;
	if(BlasterSound) ShipStatics::PlaySound(BlasterSound, this);
}

TObjectPtr<ABlasterShot> APlayerShip::SpawnBlasterShot() {
	const TObjectPtr<UArrowComponent> BarrelToFireFrom = DeterminWhichBarrelToFireFrom();
	TObjectPtr<ABlasterShot> BlasterShot {};
	if (TObjectPtr<UWorld> World = GetWorld()) {
		BlasterShot = World->SpawnActor<ABlasterShot>(
			BlasterShotBlueprintClass,
			BarrelToFireFrom->GetComponentLocation(),
			BarrelToFireFrom->GetComponentRotation()
		);
	}
	return BlasterShot;
}

TObjectPtr<UArrowComponent> APlayerShip::DeterminWhichBarrelToFireFrom() {
	switch (BarrelNumberToFireFrom) {
	    case 1:
			BarrelNumberToFireFrom = 2;
			return GunBarrel1;
		case 2:
			BarrelNumberToFireFrom = 3;
			return GunBarrel2;
		case 3:
			BarrelNumberToFireFrom = 4;
			return GunBarrel3;
		case 4:
			BarrelNumberToFireFrom = 1;
			return GunBarrel4;
	}
	return GunBarrel1;
}

void APlayerShip::SetHealthBarPercent() {
	if (PlayerShipOverlay) {
		PlayerShipOverlay->SetHealthBarPercent(PlayerShipAttributes->GetHealthPercent());
	}
}

void APlayerShip::HandleHeadLightHUDText() {
	if (PlayerShipOverlay) {
		PlayerShipOverlay->SetHeadLightText(HeadLightsAreOn());
	}
}

bool APlayerShip::HeadLightsAreOn() {
	if (LeftHeadLight && RightHeadLight) {
		return LeftHeadLight->IsVisible() && RightHeadLight->IsVisible();
	}
	return false;
}

void APlayerShip::TurnHeadLightsOn() {
	LeftHeadLight->SetVisibility(true);
	RightHeadLight->SetVisibility(true);
}

void APlayerShip::TurnHeadLightsOff() {
	LeftHeadLight->SetVisibility(false);
	RightHeadLight->SetVisibility(false);
}

TObjectPtr<USpaceInvader3DOverlay> APlayerShip::SetOverlay() {
	TObjectPtr<USpaceInvader3DOverlay> Overlay {};
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController())) {
		if (TObjectPtr<ASpaceInvader3DHUD> SpaceInvader3DHUD = Cast<ASpaceInvader3DHUD>(PlayerController->GetHUD())) {
			if (TObjectPtr<USpaceInvader3DOverlay> SpaceInvader3DOverlay = SpaceInvader3DHUD->GetSpaceInvader3DOverlay()) {
				if (PlayerShipAttributes) {
					Overlay = SpaceInvader3DOverlay;
				}
			}
		}
	}
	return Overlay;
}

void APlayerShip::SetupEnemyShipDetectionFunctionality() {
	if (EnemyShipDetectionSphere) {
		EnemyShipDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::DetectEnemyShip);
		EnemyShipDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerShip::LoseEnemyShip);
	}
}

void APlayerShip::SetupEnemyShipLockOnFunctionality() {
	if (MissleLockOnDetectionCapsule) {
		MissleLockOnDetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::HandleLockingOnToEnemyShip);
		MissleLockOnDetectionCapsule->OnComponentEndOverlap.AddDynamic(this, &APlayerShip::HandleLosingLockedEnemyShips);
	}
}

void APlayerShip::HandleLockingOnToEnemyShip(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
		PotentiallyLockedOnEnemyShip = EnemyShip;
	}
}

void APlayerShip::HandleLosingLockedEnemyShips(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
		PotentiallyLockedOnEnemyShip = nullptr;
	}
}

void APlayerShip::ExitViewModeAfterExploding() {
	InViewMode = false;
	GetController()->SetControlRotation(CameraResetTarget->GetComponentRotation());
}

void APlayerShip::UpdateEnemyShipDirectionArrowRotation() {
	if (TargetedEnemyShip && EnemyShipDirectionArrow) {
		EnemyShipDirectionArrow->SetWorldRotation(
			UKismetMathLibrary::FindLookAtRotation(
				EnemyShipDirectionArrow->GetComponentLocation(),
				TargetedEnemyShip->GetActorLocation()
			)
		);
	}
}

void APlayerShip::HandleEnemyShipDirectionArrowVisibility() {
	if (EnemyShipDirectionArrow) {
		if (TargetedEnemyShip) {
			EnemyShipDirectionArrow->SetVisibility(true);
		} else {
			EnemyShipDirectionArrow->SetVisibility(false);
		}
	}
}

void APlayerShip::HandleTargetedEnemyShipStatus() {
	if (!IsValid(TargetedEnemyShip)) {
		TargetedEnemyShip = nullptr;
	}
}

void APlayerShip::DetectEnemyShip(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
		DetectedEnemyShips.Add(EnemyShip);
	}
}

void APlayerShip::LoseEnemyShip(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (const TObjectPtr<AEnemyShip> EnemyShip = Cast<AEnemyShip>(OtherActor)) {
		DetectedEnemyShips.Remove(EnemyShip);
		if (EnemyShip == TargetedEnemyShip) TargetedEnemyShip = nullptr;
	}
}

void APlayerShip::HandleExploding() {
	if (PlayerShipAttributes) {
		if (!HasHandledExploding && PlayerShipAttributes->GetIsDead()) {
			Explode();
			DeactivateComponentsAfterExploding();
			ExitViewModeAfterExploding();
			ZeroOutCurrentControlSpeed();
			HasHandledExploding = true;
		}
	}
}

void APlayerShip::Explode() {
	ShipStatics::SpawnShipExplodingEffect(ShipExplodingEffectBlueprintClass, this);
	ShipStatics::SpawnShipPieces(ShipPiecesBlueprintClass, this);
	if (FieldSystemSpawnLocation && GetWorld()) {
		ShipStatics::SpawnShipExplodingFieldSystem(
			ShipExplodingFieldSystemBlueprintClass,
			GetWorld(),
			FieldSystemSpawnLocation->GetComponentLocation(),
			FieldSystemSpawnLocation->GetComponentRotation()
		);
	}
	if (ExplodingSound) ShipStatics::PlaySound(ExplodingSound, this);
}

void APlayerShip::DeactivateComponentsAfterExploding() {
	if (ShipMeshComponent) ShipMeshComponent->SetVisibility(false);
	if (EngineThrusterEffect1) EngineThrusterEffect1->Deactivate();
	if (EngineThrusterEffect2) EngineThrusterEffect2->Deactivate();
	if (CruisingThrusterSound) CruisingThrusterSound->Deactivate();
	if (LeftHeadLight) LeftHeadLight->SetVisibility(false);
	if (RightHeadLight) RightHeadLight->SetVisibility(false);
	if (Movement) Movement->Deactivate();
	if (InnerSight) InnerSight->SetVisibility(false);
	if (OuterSight) OuterSight->SetVisibility(false);
}

void APlayerShip::ZeroOutCurrentControlSpeed() {
	CurrentYawControlSpeed = 0.0;
	CurrentPitchControlSpeed = 0.0;
}

void APlayerShip::SetMovementComponentMaxSpeed() {
	if (Movement) {
		Movement->MaxSpeed = CurrentSpeed;
	}
}

void APlayerShip::UpdatePlayerShipRotation(const float& DeltaTime) {
	// Pitch == y
	// Roll == z
	// Yaw == x
	AddActorLocalRotation(
		FRotator(
			CurrentPitchControlSpeed * DeltaTime,
			CurrentYawControlSpeed * DeltaTime,
			0
		)
	);
}

void APlayerShip::Look(const FInputActionValue& Value) {
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (SpringArm && !PlayerShipAttributes->GetIsDead()) {
		if (InViewMode) {
			View(LookAxisValue, 0.3);
		} else {
			Steer(LookAxisValue);
		}
	}
}

void APlayerShip::View(const FVector2D& LookAxisValue, const double& Sensitivity) {
	ResetControlRotation();
	SpringArm->bUsePawnControlRotation = true;
	AddControllerYawInput(-LookAxisValue.X * Sensitivity);
	AddControllerPitchInput(LookAxisValue.Y * Sensitivity);
}

void APlayerShip::ResetControlRotation() {
	if (CameraResetTarget && GetController()) {
		if (!SpringArm->bUsePawnControlRotation) {
			GetController()->SetControlRotation(CameraResetTarget->GetComponentRotation());
		}
	}
}

void APlayerShip::Steer(const FVector2D& LookAxisValue) {
	SetCurrentControlSpeed(LookAxisValue.X, 0.1, 30.0, 0.4, CurrentYawControlSpeed);
	SetCurrentControlSpeed(LookAxisValue.Y, 0.2, 50.0, 1.4, CurrentPitchControlSpeed);
	SpringArm->bUsePawnControlRotation = false;
}

void APlayerShip::SetCurrentControlSpeed(const double& ControlSpeedInput, const double& DeadZone, const double& MaxTurnSpeed, const double& Sensitivity, double& CurrentControlSpeed) {
	if (abs(ControlSpeedInput) > DeadZone) {
		float ClampedCurrentControlSpeed = FMath::Clamp(CurrentControlSpeed, -MaxTurnSpeed, MaxTurnSpeed);
		ClampedCurrentControlSpeed -= ControlSpeedInput * Sensitivity;
		CurrentControlSpeed = ClampedCurrentControlSpeed;
	}
}

void APlayerShip::RollLeft() {
	if (!PlayerShipAttributes->GetIsDead()) {
		AddActorLocalRotation(
			FRotator(0, 0, -90 * UGameplayStatics::GetWorldDeltaSeconds(this))
		);
	}
}

void APlayerShip::RollRight() {
	if (!PlayerShipAttributes->GetIsDead()) {
		AddActorLocalRotation(
			FRotator(0, 0, 90 * UGameplayStatics::GetWorldDeltaSeconds(this))
		);
	}
}

void APlayerShip::ToggleHeadlights() {
	if (!PlayerShipAttributes->GetIsDead()) {
		if (LeftHeadLight && RightHeadLight) {
			if (HeadLightsAreOn()) {
				TurnHeadLightsOff();
			} else {
				TurnHeadLightsOn();
			}
			if(ToggleHeadLightSound) ShipStatics::PlaySound(ToggleHeadLightSound, this);
		}
	}
}

void APlayerShip::SetTargetedEnemyShip() {
	for (const TObjectPtr<AEnemyShip> EnemyShip : DetectedEnemyShips) {
		if (TargetedEnemyShip != EnemyShip) {
			TargetedEnemyShip = EnemyShip;
			break;
		}
	}
}

void APlayerShip::Accelerate() {
	if (CurrentSpeed < MaxSpeed) {
		const float DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);
		const float PercentOfMaxSpeed = (CurrentSpeed / MaxSpeed) * 100.f;
		const float PercentDifference = 100.f - PercentOfMaxSpeed;
		CurrentSpeed += ((PercentDifference * 0.2f) * (DeltaSeconds * 100.f));
	}
}

void APlayerShip::Decelerate() {
	if (CurrentSpeed > MinSpeed) {
		const float DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);
		CurrentSpeed -= (10.f * (DeltaSeconds * 100.f));
	}
}

void APlayerShip::ToggleViewMode() {
	if (GetController() && CameraResetTarget) {
		if (InViewMode) {
			InViewMode = false;
			GetController()->SetControlRotation(CameraResetTarget->GetComponentRotation());
		}
		else if(PlayerShipAttributes && !PlayerShipAttributes->GetIsDead()) {
			InViewMode = true;
		}
	}
}

void APlayerShip::SetThrusterPitch() {
	CruisingThrusterSound->SetPitchMultiplier(CurrentSpeed * 0.0001f);
}

void APlayerShip::SetThrusterColor() {
	const float Blue = (CurrentSpeed * .08f);
	const float Alpha = (CurrentSpeed * .005f);
	const FColor NewColor(90.f, 0.f, Blue, Alpha);
	EngineThrusterEffect1->SetNiagaraVariableLinearColor(FString("ParticleColor"), FLinearColor(NewColor));
	EngineThrusterEffect2->SetNiagaraVariableLinearColor(FString("ParticleColor"), FLinearColor(NewColor));
}
