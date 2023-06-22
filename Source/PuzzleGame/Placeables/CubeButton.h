// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeButton.generated.h"

class UBoxComponent;

UCLASS()
class PUZZLEGAME_API ACubeButton : public AActor
{
	GENERATED_BODY()
	
public:
	ACubeButton();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ButtonMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	bool bCubePlaced;
	
public:
	FORCEINLINE bool GetbCubePlaced() const { return bCubePlaced; }
};
