// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ventilation.generated.h"

class AScrew;

UCLASS()
class PUZZLEGAME_API AVentilation : public AActor
{
	GENERATED_BODY()
	
public:	
	AVentilation();
	virtual void Tick(float DeltaTime) override;
	void OnUnscrewed();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VentilationMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AScrew> ScrewClass;
};
