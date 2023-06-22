// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PuzzleGame/Placeables/CubeButton.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
<<<<<<< Updated upstream

	if (Button && Button->GetbCubePlaced())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("DoorOpens"));
	}
=======
>>>>>>> Stashed changes
}

