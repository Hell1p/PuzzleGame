// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSource.generated.h"

class USpotLightComponent;
class USphereComponent;
class APuzzlePlayer;

UCLASS()
class PUZZLEGAME_API ALightSource : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightSource();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* LightSourceMesh;
	
	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* SpotLight;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	APuzzlePlayer* Player;
	bool bOverlappingPlayer;

public:
	FORCEINLINE bool GetbOverlappingPlayer() const { return bOverlappingPlayer; }
};
