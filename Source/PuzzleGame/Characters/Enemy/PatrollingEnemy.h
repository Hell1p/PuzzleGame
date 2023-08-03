// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "PatrollingEnemy.generated.h"

UENUM()
enum class EEnemyState
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
	EES_Chasing UMETA(DisplayName = "Chasing")
};

UCLASS()
class PUZZLEGAME_API APatrollingEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	AActor* ChoosePatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void CheckPatrolTarget();
	
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	AActor* CombatTarget;
	FTimerHandle PatrolTimer;

	// Current patrol target
	UPROPERTY(EditInstanceOnly, Category = AINavigation)
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = AINavigation)
	TArray<AActor*> PatrolTargets;
	
	UPROPERTY(EditAnywhere, Category = EnemyStats)
	double AttackRadius = 150.f;
	UPROPERTY(EditAnywhere, Category = EnemyStats)
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = EnemyStats)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = EnemyStats)
	float WaitMin = 2.f;
	UPROPERTY(EditAnywhere, Category = EnemyStats)
	float WaitMax = 5.f;
};
