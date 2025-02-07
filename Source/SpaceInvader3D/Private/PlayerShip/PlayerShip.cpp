#include "PlayerShip/PlayerShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

APlayerShip::APlayerShip() {
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	ShipCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShipCapsule"));
	SetRootComponent(ShipCapsule);

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 1600.f;
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 260.0f);
	SpringArm->SocketOffset = FVector(3600.0f, 0.0f, 0.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
}

void APlayerShip::BeginPlay() {
	Super::BeginPlay();
	SetupMappingContext();
}

void APlayerShip::SetupMappingContext() {
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController())) {
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void APlayerShip::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->AddActorWorldOffset(FVector(10.0f, 0.0f, 0.0f));
}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerShip::Look);
		EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &APlayerShip::Accelerate);
	}
}

void APlayerShip::Look(const FInputActionValue& Value) {
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController()) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void APlayerShip::Accelerate(const FInputActionValue& Value) {
	LogMessage("Accelerate was called");
}

void APlayerShip::LogMessage(const FString& Message) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
	}
}
