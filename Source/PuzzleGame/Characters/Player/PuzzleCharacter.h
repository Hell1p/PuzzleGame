// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzleCharacter.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;
class APlayerController;
class APlayerController;
class APuzzleHUD;
class ARCCar;
class AFlashlight;
class UPlayerOverlay;
class APuzzlePlayerController;

UENUM(BlueprintType)
enum class EMovementDirectionState1 : uint8
{
	EMDS_Forward UMETA(DisplayName = "Forward"),
	EMDS_Left_Right UMETA(DisplayName = "Left_Right"),
	EMDS_Backward UMETA(DisplayName = "Backward"),

	EMDS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EPlayerToolEquippedState1 : uint8
{
	EPTES_Flashlight UMETA(DisplayName = "Flashlight"), // First slot
	EPTES_RCCar UMETA(DisplayName = "RCCar"), // Second slot

	EPTES_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PUZZLEGAME_API APuzzleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	APuzzleCharacter();
	void HandleStamina(float DeltaTime);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void RotateGrabbedObject_Right(float Value);
	void RotateGrabbedObject_Left(float Value);
	void GrabButtonPressed();
	void GrabButtonReleased();
	void SprintStart();
	void SprintEnd();
 
	UPROPERTY(BlueprintReadWrite)
	bool bCrouching;
	UPROPERTY(BlueprintReadOnly)
	bool bSprinting;
	UPROPERTY(BlueprintReadWrite)
	bool bBlockingHit;
	UPROPERTY(BlueprintReadWrite)
	bool bBlockingHitR;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMovementDirectionState1 PlayerMovementDirectionState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerToolEquippedState1 PlayerToolEquippedState;
	
	UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
	float SprintingSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
	float BaseSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
	float MaxSpeedL_R = 475.f;
	UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
	float MaxSpeedBwd = 435.f;
 
	UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
	float SprintFOV = 110.f;
	UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
	float DefaultFOV = 90.f;
	float CurrentFOV;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* PlayerCamera;

private:
	virtual void Jump() override;
	void RegenerateStamina(float DeltaTime);
	void DirectionalMovement();
	void StartSprintingWhenNeeded();
	void InteractCrosshair();
	void SetGrabbedObjectLocation();
	void InitializePuzzleOverlay();
	void SetSprintingFOV(float DeltaTime);
	void StaminaBarHide();
	void StaminaBarShow();
	void UseStamina(float StaminaCost);
	void InitializeInventoryTools();
	void FlashlightOn_Off();
	void ToggleRCCar();
	void SlotSwitch_1(); // Flashlight
	void SlotSwitch_2(); // RCCar

	bool bPrevLightTurnedOn;
	UPROPERTY(VisibleAnywhere)
	APuzzlePlayerController* PuzzlePlayerController;
	float RegenDeltaTime;
	bool bWantsToSprint;
	FTimerHandle StaminaRegenTimer;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	UPROPERTY(VisibleAnywhere)
	APlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere)
	APuzzleHUD* HUD;
	UPROPERTY(VisibleAnywhere)
	UPlayerOverlay* PlayerOverlay;
	bool bGrabbingObject;
	bool StaminaBarHidden;
	bool StaminaBarTimerStarted;
	FTimerHandle StaminaBarHideTimer;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	TSubclassOf<ARCCar> RCCarClass;
	UPROPERTY(VisibleAnywhere)
	ARCCar* RCCar;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	TSubclassOf<AFlashlight> FlashlightClass;
	UPROPERTY(VisibleAnywhere)
	AFlashlight* Flashlight;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	float GrabDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	float SprintCost = 0.25f;
	
	UPROPERTY(EditAnywhere, Category = PlayerStats)
	float MaxStamina = 100.f;

	 UPROPERTY(EditAnywhere, Category = PlayerStats)
	float StaminaRegenRate = 5.f;
 
	UPROPERTY(VisibleAnywhere, Category = PlayerStats)
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	float StaminaBar_Hide_ShowDelay = 10.f;
	
	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;

public:
	APuzzlePlayerController* GetPuzzlePlayerController();
};
