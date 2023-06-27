// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"

class USpotLightComponent;
class USkeletalMeshComponent;
class USoundBase;

UCLASS()
class PUZZLEGAME_API AFlashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlashlight();
	virtual void Tick(float DeltaTime) override;

	void LightTurnOff(bool PlaySound);
	void LightTurnOn(bool PlaySound);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FlashlightMesh;
	
	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* SpotLight;

	bool bFlashlightTurnedOn = true;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* FlashlightOnSound;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* FlashlightOffSound;

	USoundConcurrency* FootstepsConcurrency;
	
public:
	bool GetbFlashlightTurnedOn() const { return bFlashlightTurnedOn; }
	void SetbFlashlightTurnedOn(bool bTurnedOn) { bFlashlightTurnedOn = bTurnedOn; }
	USkeletalMeshComponent* GetFlashlightMesh() const { return FlashlightMesh; }
};
