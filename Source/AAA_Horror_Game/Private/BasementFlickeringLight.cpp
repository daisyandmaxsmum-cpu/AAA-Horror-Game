#include "BasementFlickeringLight.h"
#include "Components/PointLightComponent.h"

ABasementFlickeringLight::ABasementFlickeringLight()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.05f; // Update every 50ms for smooth transitions

	// Don't replicate this actor over the network
	bReplicates = false;

	// Create the root point light component
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetMobility(EComponentMobility::Stationary);
	PointLight->SetIntensity(MaxIntensity);
	PointLight->SetLightColor(LightColor);
	PointLight->SetAttenuationRadius(1500.0f);
	PointLight->SetSourceRadius(5.0f);
	RootComponent = PointLight;

	// Set initial light state
	bLightIsOn = true;
}

void ABasementFlickeringLight::BeginPlay()
{
	Super::BeginPlay();

	if (!PointLight)
	{
		UE_LOG(LogTemp, Error, TEXT("BasementFlickeringLight: PointLight component not found!"));
		return;
	}

	// Ensure light color is set
	PointLight->SetLightColor(LightColor);

	// Start flickering if enabled
	if (bIsFlickering)
	{
		StartFlickering();
	}
}

void ABasementFlickeringLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Additional per-frame logic can be added here if needed
	// Currently, most logic is handled by timers
}

void ABasementFlickeringLight::ToggleLightFlicker()
{
	if (!bIsFlickering)
	{
		return;
	}

	bLightIsOn = !bLightIsOn;

	// Set the light intensity based on current state
	if (bLightIsOn)
	{
		SetLightIntensity(MaxIntensity, true);
	}
	else
	{
		SetLightIntensity(MinIntensity, true);
	}

	// Schedule the next flicker
	ResetFlickerTimer();
}

float ABasementFlickeringLight::GetRandomFlickerInterval(float BaseDuration)
{
	// Add randomness to the flicker timing
	if (RandomVariance > 0.0f && RandomVariance <= 1.0f)
	{
		float RandomOffset = FMath::RandRange(-BaseDuration * RandomVariance, BaseDuration * RandomVariance);
		return FMath::Max(0.01f, BaseDuration + RandomOffset); // Ensure minimum duration
	}
	return BaseDuration;
}

void ABasementFlickeringLight::SetLightIntensity(float Intensity, bool bSmooth)
{
	if (!PointLight)
	{
		return;
	}

	if (bSmooth)
	{
		// Use a simple lerp-based smooth transition (could be enhanced with curves)
		PointLight->SetIntensity(Intensity);
	}
	else
	{
		PointLight->SetIntensity(Intensity);
	}
}

void ABasementFlickeringLight::StartFlickering()
{
	if (!PointLight)
	{
		return;
	}

	bIsFlickering = true;
	bLightIsOn = true;
	SetLightIntensity(MaxIntensity);

	// Start the first flicker cycle
	ResetFlickerTimer();

	UE_LOG(LogTemp, Warning, TEXT("BasementFlickeringLight: Flickering started"));
}

void ABasementFlickeringLight::StopFlickering()
{
	if (!PointLight)
	{
		return;
	}

	bIsFlickering = false;

	// Clear the timer
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
	}

	// Set light to full brightness
	SetLightIntensity(MaxIntensity);
	bLightIsOn = true;

	UE_LOG(LogTemp, Warning, TEXT("BasementFlickeringLight: Flickering stopped"));
}

void ABasementFlickeringLight::ResetFlickerTimer()
{
	if (!GetWorld())
	{
		return;
	}

	// Clear existing timer
	GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);

	if (!bIsFlickering)
	{
		return;
	}

	// Calculate the next interval based on current light state
	float NextInterval = bLightIsOn ? GetRandomFlickerInterval(OnDuration) : GetRandomFlickerInterval(OffDuration);

	// Set the timer for the next flicker toggle
	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimerHandle,
		this,
		&ABasementFlickeringLight::ToggleLightFlicker,
		NextInterval,
		false
	);
}

float ABasementFlickeringLight::GetCurrentIntensity() const
{
	if (PointLight)
	{
		return PointLight->Intensity;
	}
	return 0.0f;
}

void ABasementFlickeringLight::SetFlickeringEnabled(bool bEnabled)
{
	if (bEnabled == bIsFlickering)
	{
		return; // Already in desired state
	}

	if (bEnabled)
	{
		StartFlickering();
	}
	else
	{
		StopFlickering();
	}
}

void ABasementFlickeringLight::TurnLightOn()
{
	if (!PointLight)
	{
		return;
	}

	// Temporarily disable flickering to allow manual control
	bool bWasFlickering = bIsFlickering;
	bIsFlickering = false;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
	}

	SetLightIntensity(MaxIntensity);
	bLightIsOn = true;

	// Restore flickering if it was active
	if (bWasFlickering)
	{
		bIsFlickering = true;
		ResetFlickerTimer();
	}

	UE_LOG(LogTemp, Warning, TEXT("BasementFlickeringLight: Light turned on"));
}

void ABasementFlickeringLight::TurnLightOff()
{
	if (!PointLight)
	{
		return;
	}

	// Temporarily disable flickering to allow manual control
	bool bWasFlickering = bIsFlickering;
	bIsFlickering = false;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FlickerTimerHandle);
	}

	SetLightIntensity(MinIntensity);
	bLightIsOn = false;

	// Restore flickering if it was active
	if (bWasFlickering)
	{
		bIsFlickering = true;
		ResetFlickerTimer();
	}

	UE_LOG(LogTemp, Warning, TEXT("BasementFlickeringLight: Light turned off"));
}
