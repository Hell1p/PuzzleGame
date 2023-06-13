#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzlePlayer.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;
class APlayerController;
class APlayerController;
class APuzzleHUD;
class UPlayerOverlay;

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
 void MoveForward(float Value);
 void MoveRight(float Value);
 void LookUp(float Value);
 void Turn(float Value);
 void GrabButtonPressed();
 void GrabButtonReleased();
 void SprintStart();
 void SprintEnd();
 
 UPROPERTY(BlueprintReadWrite)
 bool bCrouching;
 UPROPERTY(BlueprintReadOnly)
 bool bSprinting;
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
 float SprintFOV = 110;
 UPROPERTY(EditAnywhere, Category = Movement, BlueprintReadOnly)
 float DefaultFOV = 90.f;
 float CurrentFOV;
 
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
 UCameraComponent* PlayerCamera;

private:
 void RegenerateStamina(float DeltaTime);
 void DirectionalMovement();
 void StartSprintingWhenNeeded();
 void InteractCrosshair();
 void SetGrabbedObjectLocation();
 void InitializePuzzleOverlay();
 void SetSprintingFOV(float DeltaTime);
 void UseStamina(float StaminaCost);

 float RegenDeltaTime;
 bool bWantsToSprint;
 FTimerHandle StaminaRegenTimer;

 UPROPERTY(VisibleAnywhere)
 UPhysicsHandleComponent* PhysicsHandle;

 UPROPERTY(EditAnywhere, Category = PlayerStats)
 float GrabDistance = 300.f;

 UPROPERTY(EditAnywhere, Category = PlayerStats)
 float SprintCost = 0.5f;
 
 UPROPERTY(EditAnywhere, Category = PlayerStats)
 float MaxStamina = 100.f;

 UPROPERTY(EditAnywhere, Category = PlayerStats)
 float StaminaRegenRate = 5.f;
 
 UPROPERTY(VisibleAnywhere, Category = PlayerStats)
 float CurrentStamina;

 UPROPERTY(EditAnywhere, Category = PlayerStats)
 float StaminaBar_Hide_ShowDelay = 10.f;
 
 bool StaminaBarHidden;
 bool StaminaBarTimerStarted;
 FTimerHandle StaminaBarHideTimer;
 void StaminaBarHide();
 void StaminaBarShow();
 
 APlayerController* PlayerController;
 APuzzleHUD* HUD;
 UPlayerOverlay* PlayerOverlay;
 bool bGrabbingObject;
};