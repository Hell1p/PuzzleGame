// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"

class USpotLightComponent;
class USkeletalMeshComponent;

UCLASS()
class PUZZLEGAME_API AFlashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlashlight();
	virtual void Tick(float DeltaTime) override;

	void LightTurnOff();
	void LightTurnOn();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FlashlightMesh;
	
	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* SpotLight;

	bool bFlashlightTurnedOn = true;

public:
	bool GetbFlashlightTurnedOn() const { return bFlashlightTurnedOn; }
	void SetbFlashlightTurnedOn(bool bTurnedOn) { bFlashlightTurnedOn = bTurnedOn; }
	USkeletalMeshComponent* GetFlashlightMesh() const { return FlashlightMesh; }
};
