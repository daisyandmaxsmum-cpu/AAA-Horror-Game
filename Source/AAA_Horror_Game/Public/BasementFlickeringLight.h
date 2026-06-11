#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "BasementFlickeringLight.generated.h"

/**
 * BasementFlickeringLight - Creates a flickering light effect for horror atmosphere
 * Used in basement scenes to create an unsettling, unreliable light source
 */
UCLASS()
class AAA_HORROR_GAME_API ABasementFlickeringLight : public AActor
{
	GENERATED_BODY()

public:
	ABasementFlickeringLight();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * The point light component that will flicker
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lighting")
	class UPointLightComponent* PointLight;

	/**
	 * Intensity of the light at full brightness
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering")
	float MaxIntensity = 1500.0f;

	/**
	 * Minimum intensity during flicker
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering")
	float MinIntensity = 300.0f;

	/**
	 * How long the light stays on (in seconds)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering")
	float OnDuration = 0.1f;

	/**
	 * How long the light stays off (in seconds)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering")
	float OffDuration = 0.15f;

	/**
	 * Random variance for flicker timing (0.0-1.0)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RandomVariance = 0.3f;

	/**
	 * Enable/Disable flickering effect
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering")
	bool bIsFlickering = true;

	/**
	 * Current state of the light
	 */
	bool bLightIsOn = true;

	/**
	 * Timer for managing flicker timing
	 */
	FTimerHandle FlickerTimerHandle;

	/**
	 * Color tint of the flickering light (for horror ambiance)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flickering")
	FLinearColor LightColor = FLinearColor(1.0f, 0.85f, 0.6f); // Warm yellow-orange

	/**
	 * Called when flicker timer triggers
	 */
	void ToggleLightFlicker();

	/**
	 * Calculates the next flicker interval with randomness
	 */
	float GetRandomFlickerInterval(float BaseDuration);

	/**
	 * Set the light intensity with optional smoothing
	 */
	void SetLightIntensity(float Intensity, bool bSmooth = false);

	/**
	 * Start the flickering effect
	 */
	UFUNCTION(BlueprintCallable, Category = "Flickering")
	void StartFlickering();

	/**
	 * Stop the flickering effect and set light to max intensity
	 */
	UFUNCTION(BlueprintCallable, Category = "Flickering")
	void StopFlickering();

	/**
	 * Reset the flickering timer
	 */
	void ResetFlickerTimer();

public:
	/**
	 * Get current light intensity
	 */
	UFUNCTION(BlueprintCallable, Category = "Flickering")
	float GetCurrentIntensity() const;

	/**
	 * Set flickering enabled/disabled
	 */
	UFUNCTION(BlueprintCallable, Category = "Flickering")
	void SetFlickeringEnabled(bool bEnabled);

	/**
	 * Instantly turn the light on
	 */
	UFUNCTION(BlueprintCallable, Category = "Flickering")
	void TurnLightOn();

	/**
	 * Instantly turn the light off
	 */
	UFUNCTION(BlueprintCallable, Category = "Flickering")
	void TurnLightOff();
};
