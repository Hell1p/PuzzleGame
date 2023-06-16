// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "RCCar.generated.h"

class APuzzlePlayer;
class APuzzlePlayerController;
class UCameraComponent;

UENUM(BlueprintType)
enum class ERCCarState : uint8
{
	ERCCS_Initial UMETA(DisplayName = "Initial"),
	ERCCS_Possessed UMETA(DisplayName = "Possessed"),

	ERCCS_DefaultMAX UMETA(DisplaName = "DefaultMAX")
};

UCLASS()
class PUZZLEGAME_API ARCCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()
public:
	ARCCar();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Activate(APuzzlePlayer* Player, APuzzlePlayerController* CharacterController);
	
protected:
	virtual void BeginPlay() override;
    
	void Throttle(float Value);
	void Brake(float Value);
	void Steering(float Value);
	void HandbrakeStart();
	void HandbrakeEnd();

private:
	void AttachMeshToPuzzlePlayer();
	void Deactivate();
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ERCCarState RCCarState = ERCCarState::ERCCS_Initial;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* RCCarCamera;
	
	APuzzlePlayer* ControllingPlayer;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RCControllerMesh;
};
