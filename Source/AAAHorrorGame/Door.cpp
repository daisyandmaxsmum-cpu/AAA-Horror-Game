#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create left door mesh
	DoorMeshLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshLeft"));
	DoorMeshLeft->SetupAttachment(RootComponent);
	DoorMeshLeft->SetRelativeLocation(FVector(-50.0f, 0.0f, 0.0f));

	// Create right door mesh
	DoorMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshRight"));
	DoorMeshRight->SetupAttachment(RootComponent);
	DoorMeshRight->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	// Store initial positions of door halves
	if (DoorMeshLeft)
	{
		LeftHalfStartPos = DoorMeshLeft->GetRelativeLocation();
	}

	if (DoorMeshRight)
	{
		RightHalfStartPos = DoorMeshRight->GetRelativeLocation();
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update break animation if door is breaking
	if (bIsBroken && ElapsedTime < BreakDuration)
	{
		UpdateBreakAnimation(DeltaTime);
	}
}

void ADoor::UpdateBreakAnimation(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	// Calculate interpolation value (0 to 1)
	float Alpha = FMath::Clamp(ElapsedTime / BreakDuration, 0.0f, 1.0f);

	// Apply easing (ease-out cubic for smooth deceleration)
	Alpha = 1.0f - FMath::Pow(1.0f - Alpha, 3.0f);

	if (DoorMeshLeft)
	{
		// Move left half to the left
		FVector LeftNewPos = LeftHalfStartPos;
		LeftNewPos.X -= HalfSeparationDistance * Alpha;
		DoorMeshLeft->SetRelativeLocation(LeftNewPos);
	}

	if (DoorMeshRight)
	{
		// Move right half to the right
		FVector RightNewPos = RightHalfStartPos;
		RightNewPos.X += HalfSeparationDistance * Alpha;
		DoorMeshRight->SetRelativeLocation(RightNewPos);
	}
}

void ADoor::PlayCrackSound()
{
	if (CrackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CrackSound, GetActorLocation());
	}
}

void ADoor::PlayBreakSound()
{
	if (BreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSound, GetActorLocation());
	}
}

void ADoor::PlayMysteriousSound()
{
	if (MysteriousSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), MysteriousSound, GetActorLocation());
	}
}

void ADoor::BreakDoor()
{
	if (!bIsBroken)
	{
		bIsBroken = true;
		ElapsedTime = 0.0f;

		// Play sound effects
		PlayCrackSound();

		// Delay the break sound slightly for dramatic effect
		GetWorld()->GetTimerManager().SetTimer(
			FTimerHandle(),
			this,
			&ADoor::PlayBreakSound,
			0.5f,
			false
		);

		// Play mysterious sound
		PlayMysteriousSound();
	}
}

void ADoor::ResetDoor()
{
	bIsBroken = false;
	ElapsedTime = 0.0f;

	// Reset door halves to original positions
	if (DoorMeshLeft)
	{
		DoorMeshLeft->SetRelativeLocation(LeftHalfStartPos);
	}

	if (DoorMeshRight)
	{
		DoorMeshRight->SetRelativeLocation(RightHalfStartPos);
	}
}
