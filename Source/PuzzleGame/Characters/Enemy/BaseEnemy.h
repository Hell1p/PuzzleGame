// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class AAIController;
class UPawnSensingComponent;

UCLASS()
class PUZZLEGAME_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void MoveToTarget(AActor* Target);
	bool InTargetRange(AActor* Target, double Radius);
	void OnBeginChaseTimerEnd();
 
	AActor* PuzzlePlayerActor;
	AAIController* EnemyController;
	UPROPERTY(EditAnywhere, Category = EnemyProperties)
	float TargetRangeRadius = 90.f;
	bool bChasing;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

private:
};
