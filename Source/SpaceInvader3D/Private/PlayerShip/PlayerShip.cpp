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

APlayerShip::APlayerShip() {
	PrimaryActorTick.bCanEverTick = true;
	bFireCooldownTimerFinished = true;
	bHasPlayedExplodingSound = false;
	CurrentPitchControlSpeed = 0.0;

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
	SpringArm->TargetArmLength = 1600.f;
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

	BarrelNumberToFireFrom = 1;

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));

	MaxSpeed = 12000.0f;
	MinSpeed = 3300.0f;

	EngineThrusterEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 1"));
	EngineThrusterEffect1->SetupAttachment(GetRootComponent());

	EngineThrusterEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 2"));
	EngineThrusterEffect2->SetupAttachment(GetRootComponent());

	CruisingThrusterSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Cruising Thruster Sound"));
	CruisingThrusterSound->SetVolumeMultiplier(0.02f);
}

void APlayerShip::BeginPlay() {
	Super::BeginPlay();
	SetupMappingContext();
	SetInitialSpeed();
	PlayerShipOverlay = SetOverlay();
	SetHealthBarPercent();
}

void APlayerShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AddMovementInput(GetActorForwardVector(), 1.0f);
	SetThrusterPitch();
	SetThrusterColor();
	UpdateSpeed();
	SetHealthBarPercent();
	HandleExplodingSound();
	UpdatePlayerShipLocalRotation(DeltaTime);
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
	}
}

void APlayerShip::HandleFireTimer() {
	if (bFireCooldownTimerFinished) {
		GetWorldTimerManager().ClearTimer(FireCooldownTimer);
		GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &APlayerShip::Fire, 0.15f);
		bFireCooldownTimerFinished = false;
	}
}

void APlayerShip::Fire() {
	if (const TObjectPtr<ABlasterShot> BlasterShot = SpawnBlasterShot()) {
		BlasterShot->FireInDirection(GetActorRotation().Vector());
	}
	bFireCooldownTimerFinished = true;
	PlayBlasterSound();
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

void APlayerShip::SetInitialSpeed() {
	if (Movement) {
		Movement->MaxSpeed = MinSpeed;
	}
}

void APlayerShip::SetHealthBarPercent() {
	if (PlayerShipOverlay) {
		PlayerShipOverlay->SetHealthBarPercent(PlayerShipAttributes->GetHealthPercent());
	}
}

void APlayerShip::UpdateSpeed() {
	if (PlayerShipAttributes) {
		PlayerShipAttributes->SetCurrentSpeed(Movement->MaxSpeed);
	}
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

void APlayerShip::PlayBlasterSound() {
	UGameplayStatics::PlaySoundAtLocation(
		this,
		BlasterSound,
		GetActorLocation()
	);
}

void APlayerShip::HandleExplodingSound() {
	if (PlayerShipAttributes) {
		if (!bHasPlayedExplodingSound && PlayerShipAttributes->GetbHasBlownUp()) {
			PlayExplodingSound();
			bHasPlayedExplodingSound = true;
		}
	}
}

void APlayerShip::PlayExplodingSound() {
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ExplodingSound,
		GetActorLocation()
	);
}

void APlayerShip::UpdatePlayerShipLocalRotation(const float& DeltaTime) {
	// Pitch == y
	// Roll == z
	// Yaw == x
	AddActorLocalRotation(FRotator(CurrentPitchControlSpeed * DeltaTime, 0, 0));
}

void APlayerShip::Look(const FInputActionValue& Value) {
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController()) {
		CurrentPitchControlSpeed = LookAxisValue.Y * -500.0;
		AddControllerYawInput(LookAxisValue.X);
	}
}

void APlayerShip::RollLeft() {
	AddActorLocalRotation(FRotator(0, 0, -0.8));
}

void APlayerShip::RollRight() {
	AddActorLocalRotation(FRotator(0, 0, 0.8));
}

void APlayerShip::Accelerate() {
	if (Movement && Movement->MaxSpeed < MaxSpeed) {
		const float DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);
		const float PercentOfMaxSpeed = (Movement->MaxSpeed / MaxSpeed) * 100.f;
		const float PercentDifference = 100.f - PercentOfMaxSpeed;
		Movement->MaxSpeed += ((PercentDifference * 0.2f) * (DeltaSeconds * 100.f));
	}
}

void APlayerShip::Decelerate() {
	if (Movement && Movement->MaxSpeed > MinSpeed) {
		const float DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);
		Movement->MaxSpeed -= (10.f * (DeltaSeconds * 100.f));
	}
}

void APlayerShip::ToggleViewMode() {
	if (GetController() && CameraResetTarget) {
		GetController()->SetControlRotation(CameraResetTarget->GetComponentRotation());
		bUseControllerRotationPitch = ~bUseControllerRotationPitch;
		bUseControllerRotationYaw = ~bUseControllerRotationYaw;
		if (bUseControllerRotationPitch && bUseControllerRotationYaw) {
			SpringArm->TargetOffset = FVector(0.0f, 0.0f, 450.0f);
		}
		else {
			SpringArm->TargetOffset = FVector(0.0f, 0.0f, 350.0f);
		}
	}
}

void APlayerShip::SetThrusterPitch() {
	CruisingThrusterSound->SetPitchMultiplier(Movement->MaxSpeed * 0.0001f);
}

void APlayerShip::SetThrusterColor() {
	const float Blue = (Movement->MaxSpeed * .08f);
	const float Alpha = (Movement->MaxSpeed * .005f);
	const FColor NewColor(90.f, 0.f, Blue, Alpha);
	EngineThrusterEffect1->SetNiagaraVariableLinearColor(FString("ParticleColor"), FLinearColor(NewColor));
	EngineThrusterEffect2->SetNiagaraVariableLinearColor(FString("ParticleColor"), FLinearColor(NewColor));
}
