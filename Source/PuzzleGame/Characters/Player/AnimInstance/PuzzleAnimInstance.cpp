// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PuzzleGame/Characters/Player/PuzzlePlayer.h"

void UPuzzleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PuzzlePlayer = Cast<APuzzlePlayer>(TryGetPawnOwner());
}

void UPuzzleAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PuzzlePlayer == nullptr) PuzzlePlayer = Cast<APuzzlePlayer>(TryGetPawnOwner());
	if (PuzzlePlayer == nullptr) return;
	
	Speed = PuzzlePlayer->GetVelocity().Size();
	bIsCrouching = PuzzlePlayer->GetbCrouching();
	bIsSprinting = PuzzlePlayer->GetbSprinting();
	Direction = CalculateDirection(PuzzlePlayer->GetVelocity(), PuzzlePlayer->GetActorRotation());
	bIsInAir = PuzzlePlayer->GetCharacterMovement()->IsFalling();
}
