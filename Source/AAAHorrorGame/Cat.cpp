#include "Cat.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ACat::ACat()
{
	// Don't rotate the character with the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 400.0f;

	// Create camera boom (pulls the camera back behind the cat)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACat::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACat::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACat::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACat::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ACat::StartSprinting);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACat::StopSprinting);
	}
	else if (UInputComponent* StandardInputComponent = PlayerInputComponent)
	{
		// Fallback for standard input component
		StandardInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		StandardInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	}
}

void ACat::Move(const FInputActionValue& Value)
{
	// Input is a value type that contains 2 axis values (forward/backward, left/right)
	if (const FVector2D MovementVector = Value.Get<FVector2D>(); Controller != nullptr)
	{
		// Find forward direction
		if (MovementVector.Y != 0.0f)
		{
			const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, MovementVector.Y);
		}

		// Find right direction
		if (MovementVector.X != 0.0f)
		{
			const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, MovementVector.X);
		}
	}
}

void ACat::Look(const FInputActionValue& Value)
{
	// Input is a value type that contains 2 axis values (look up/down - Y, and look left/right - X)
	if (const FVector2D LookAxisVector = Value.Get<FVector2D>(); Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACat::StartSprinting()
{
	if (GetCharacterMovement())
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
	}
}

void ACat::StopSprinting()
{
	if (GetCharacterMovement())
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

float ACat::GetCurrentMovementSpeed() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->Velocity.Length();
	}
	return 0.0f;
}

void ACat::Jump()
{
	ACharacter::Jump();
}

void ACat::StopMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
}
