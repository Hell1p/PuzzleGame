// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PuzzleGame/Characters/Player/PuzzlePlayer.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

