// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Screw.generated.h"

UCLASS()
class PUZZLEGAME_API AScrew : public AActor
{
	GENERATED_BODY()
	
public:	
	AScrew();
	virtual void Tick(float DeltaTime) override;
	void Unscrew();

protected:
	virtual void BeginPlay() override;

private:
	bool bUnscrewed;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ScrewMesh;
};
