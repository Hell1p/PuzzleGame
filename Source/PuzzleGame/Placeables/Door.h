// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ACubeButton;

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
	UPROPERTY(EditInstanceOnly, Category = DoorProperties)
	ACubeButton* Button;
	
public:
	
};
