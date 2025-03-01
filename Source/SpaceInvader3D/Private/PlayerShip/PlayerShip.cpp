#include "PlayerShip/PlayerShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
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

APlayerShip::APlayerShip() {
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	ShipSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ShipSphere"));
	SetRootComponent(ShipSphereComponent);
	ShipSphereComponent->SetSphereRadius(250);
	ShipSphereComponent->SetNotifyRigidBodyCollision(true);
	ShipSphereComponent->SetCollisionProfileName(FName("Custom"));
	ShipSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipSphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ShipSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ShipSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(GetRootComponent());
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 1600.f;
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 450.0f);
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 2.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 6.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Attributes = CreateDefaultSubobject<UPlayerShipAttributes>(TEXT("Attributes"));

	CameraResetTarget = CreateDefaultSubobject<UArrowComponent>(TEXT("Camera Reset Target"));
	CameraResetTarget->SetupAttachment(GetRootComponent());
	CameraResetTarget->SetRelativeLocation(FVector(-1600.0f, 0.0f, 450.0f));

	LeftGunBarrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Gun Barrel"));
	LeftGunBarrel->SetupAttachment(GetRootComponent());

	RightGunBarrel = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Gun Barrel"));
	RightGunBarrel->SetupAttachment(GetRootComponent());

	LeftGunCanFire = false;

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));

	MaxSpeed = 12000.0f;
	MinSpeed = 3300.0f;

	EngineThrusterEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 1"));
	EngineThrusterEffect1->SetupAttachment(GetRootComponent());

	EngineThrusterEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 2"));
	EngineThrusterEffect2->SetupAttachment(GetRootComponent());

	EngineThrusterEffect3 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 3"));
	EngineThrusterEffect3->SetupAttachment(GetRootComponent());

	EngineThrusterEffect4 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 4"));
	EngineThrusterEffect4->SetupAttachment(GetRootComponent());

	CruisingThrusterSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Cruising Thruster Sound"));
	CruisingThrusterSound->SetVolumeMultiplier(0.02f);

	FireCooldownTimerFinished = true;
}

void APlayerShip::BeginPlay() {
	Super::BeginPlay();
	SetupMappingContext();
	Movement->MaxSpeed = MinSpeed;
}

void APlayerShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AddMovementInput(GetActorForwardVector(), 1.0f);
	SetThrusterPitch();
	SetThrusterColor();
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
	}
}

void APlayerShip::HandleFireTimer() {
	if (FireCooldownTimerFinished) {
		GetWorldTimerManager().ClearTimer(FireCooldownTimer);
		GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &APlayerShip::Fire, 0.15f);
		FireCooldownTimerFinished = false;
	}
}

void APlayerShip::Fire() {
	if (const TObjectPtr<ABlasterShot> BlasterShot = SpawnBlasterShot()) {
		BlasterShot->FireInDirection(GetActorRotation().Vector());
	}
	FireCooldownTimerFinished = true;
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
	if (LeftGunCanFire) {
		LeftGunCanFire = false;
		return LeftGunBarrel;
	}
	else {
		LeftGunCanFire = true;
		return RightGunBarrel;
	}
}

void APlayerShip::PlayBlasterSound() {
	UGameplayStatics::PlaySoundAtLocation(
		this,
		BlasterSound,
		GetActorLocation()
	);
}

void APlayerShip::Look(const FInputActionValue& Value) {
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
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
	EngineThrusterEffect3->SetNiagaraVariableLinearColor(FString("ParticleColor"), FLinearColor(NewColor));
	EngineThrusterEffect4->SetNiagaraVariableLinearColor(FString("ParticleColor"), FLinearColor(NewColor));
}

void APlayerShip::LogMessage(const FString& Message) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}

void APlayerShip::LogFloat(const float& Float) {
	FString Message = FString::Printf(TEXT("THE FLOAT VALUE IS: %f"), Float);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}
