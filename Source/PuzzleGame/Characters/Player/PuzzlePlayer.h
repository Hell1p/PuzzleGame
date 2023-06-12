#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzlePlayer.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;
class APlayerController;
class APuzzleHUD;

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
	void SetSprintingFOV(float DeltaTime);
	
	UPROPERTY(BlueprintReadWrite)
	bool bCrouching;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCameraComponent* PlayerCamera;
	
private:
	void DirectionalMovement();
		
	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere)
	float GrabDistance = 300.f;
	//vsem pr i roblox player 
	APlayerController* PlayerController;
	APuzzleHUD* HUD;
	bool bGrabbingObject;
	bool bSprinting;

	float CurrentFOV = 90.f;
	float SprintFOV = 110;
	float DefaultFOV = 90.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeedL_R = 475.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeedBwd = 435.f;


};