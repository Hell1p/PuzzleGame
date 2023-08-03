// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "AITypes.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/GameplayStatics.h"
#include "PuzzleGame/Characters/Player/PuzzlePlayer.h"
#include "Navigation/PathFollowingComponent.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AAIController>(GetController());

	FTimerHandle BeginChaseTimer;
	GetWorldTimerManager().SetTimer(BeginChaseTimer, this, &ABaseEnemy::OnBeginChaseTimerEnd, 1.f);
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PuzzlePlayerActor)
	{
		if (!InTargetRange(PuzzlePlayerActor, TargetRangeRadius) && !bChasing)
		{
			MoveToTarget(PuzzlePlayerActor);
		}
		else
		{
			bChasing = false;
		}
	}
}

void ABaseEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	bChasing = true;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(25.f);
	EnemyController->MoveTo(MoveRequest);
}

bool ABaseEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void ABaseEnemy::OnBeginChaseTimerEnd()
{
	PuzzlePlayerActor = UGameplayStatics::GetActorOfClass(this, APuzzlePlayer::StaticClass());
	if (PuzzlePlayerActor) MoveToTarget(PuzzlePlayerActor);
}
