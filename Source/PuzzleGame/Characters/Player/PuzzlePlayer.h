#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzlePlayer.generated.h"

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
enum class EMovementDirectionState : uint8
{
 EMDS_Forward UMETA(DisplayName = "Forward"),
 EMDS_Left_Right UMETA(DisplayName = "Left_Right"),
 EMDS_Backward UMETA(DisplayName = "Backward"),

 EMDS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PUZZLEGAME_API APuzzlePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	APuzzlePlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void HandleStamina(float DeltaTime);
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
	bool bBlockingHitR;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMovementDirectionState PlayerMovementDirectionState;

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

	APuzzlePlayerController* PuzzlePlayerController;
	float RegenDeltaTime;
	bool bWantsToSprint;
	FTimerHandle StaminaRegenTimer;
	float AO_Pitch;
	FRotator StartingAimRotation;
	APlayerController* PlayerController;
	APuzzleHUD* HUD;
	UPlayerOverlay* PlayerOverlay;
	bool bGrabbingObject;
	bool StaminaBarHidden;
	bool StaminaBarTimerStarted;
	FTimerHandle StaminaBarHideTimer;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	TSubclassOf<ARCCar> RCCarClass;
	ARCCar* RCCar;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	TSubclassOf<AFlashlight> FlashlightClass;
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
