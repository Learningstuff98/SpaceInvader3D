#include "PlayerShip/PlayerShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"

APlayerShip::APlayerShip() {
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	ShipCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShipCapsule"));
	SetRootComponent(ShipCapsule);

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(GetRootComponent());
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 1600.f;
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 450.0f);
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 6.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 12.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CameraResetTarget = CreateDefaultSubobject<UArrowComponent>(TEXT("Camera Reset Target"));
	CameraResetTarget->SetupAttachment(GetRootComponent());
	CameraResetTarget->SetRelativeLocation(FVector(-1600.0f, 0.0f, 450.0f));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));

	MaxSpeed = 6000.0f;
	MinSpeed = 2000.0f;
	Movement->MaxSpeed = 4000.0f;

	EngineThrusterEffect1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 1"));
	EngineThrusterEffect1->SetupAttachment(GetRootComponent());

	EngineThrusterEffect2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 2"));
	EngineThrusterEffect2->SetupAttachment(GetRootComponent());

	EngineThrusterEffect3 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 3"));
	EngineThrusterEffect3->SetupAttachment(GetRootComponent());

	EngineThrusterEffect4 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Engine Thruster Effect 4"));
	EngineThrusterEffect4->SetupAttachment(GetRootComponent());
}

void APlayerShip::BeginPlay() {
	Super::BeginPlay();
	SetupMappingContext();
}

void APlayerShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AddMovementInput(GetActorForwardVector(), 1.0f);
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
	}
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
		Movement->MaxSpeed = MaxSpeed;
	}
}

void APlayerShip::Decelerate() {
	if (Movement && Movement->MaxSpeed > MinSpeed) {
		Movement->MaxSpeed = MinSpeed;
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

void APlayerShip::LogMessage(const FString& Message) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}
