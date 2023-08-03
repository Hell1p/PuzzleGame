// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingEnemy.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void APatrollingEnemy::BeginPlay()
{
	ACharacter::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("BeginPlay"));
	if (PatrolTarget == nullptr) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("nullptr"));
	EnemyController = Cast<AAIController>(GetController());
	
	PawnSensing->OnSeePawn.AddDynamic(this, &APatrollingEnemy::PawnSeen);
	MoveToTarget(PatrolTarget);
}

void APatrollingEnemy::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, TEXT("CheckPatrolTarget"));
		CheckPatrolTarget();
	}
}

void APatrollingEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &APatrollingEnemy::PatrolTimerFinished, WaitTime);
	}
}

void APatrollingEnemy::CheckCombatTarget()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("CheckCombatTarget"));
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		// Outside combat radius, lose interest
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
		UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("ChasePlayer"));
		// Outside attack range, chase character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (InTargetRange(CombatTarget, AttackRadius))
	{
		// Inside attack range, attack character
		// TODO: Attack montage
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
}

void APatrollingEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void APatrollingEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) return;
	if (SeenPawn->ActorHasTag(FName("PuzzlePlayer")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;
		EnemyState = EEnemyState::EES_Engaged;
	}
}

AActor* APatrollingEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}
