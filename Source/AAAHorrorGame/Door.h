#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class USoundBase;
class UStaticMeshComponent;

/**
 * ADoor - A destructible door that breaks in half with mysterious sound effects
 */
UCLASS()
class AAAHORORGAME_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	// Static mesh components for the door halves
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMeshLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMeshRight;

	// Sound effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* CrackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* BreakSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* MysteriousSound;

	// Door state
	UPROPERTY(BlueprintReadOnly, Category = "Door")
	bool bIsBroken = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float BreakDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float HalfSeparationDistance = 50.0f;

	// Animation
	float ElapsedTime = 0.0f;
	FVector LeftHalfStartPos;
	FVector RightHalfStartPos;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Internal break animation logic
	void UpdateBreakAnimation(float DeltaTime);

	// Sound playback
	void PlayCrackSound();
	void PlayBreakSound();
	void PlayMysteriousSound();

public:
	// Function to initiate door break sequence
	UFUNCTION(BlueprintCallable, Category = "Door")
	void BreakDoor();

	// Function to reset door to intact state
	UFUNCTION(BlueprintCallable, Category = "Door")
	void ResetDoor();

	// Getter for door state
	UFUNCTION(BlueprintCallable, Category = "Door")
	bool GetIsBroken() const { return bIsBroken; }
};
