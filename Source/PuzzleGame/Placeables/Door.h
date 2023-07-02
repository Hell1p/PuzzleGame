// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ACubeButton;
class ACardReader;

UCLASS()
class PUZZLEGAME_API ADoor : public AActor
{
	GENERATED_BODY()

public:	
	ADoor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* DoorMesh;
	
	UPROPERTY(EditInstanceOnly, Category = DoorProperties)
	ACubeButton* Button;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FrameMesh;

public:
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetLeftDoorMesh() const { return LeftDoorMesh; }

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetRightDoorMesh() const { return RightDoorMesh; }
};
