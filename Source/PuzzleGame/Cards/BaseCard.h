// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCard.generated.h"

UCLASS()
class PUZZLEGAME_API ABaseCard : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseCard();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CardMesh;
};
