// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PuzzleGame/Placeables/CubeButton.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
/*
	if (Button && Button->GetbCubePlaced())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("DoorOpens"));
	}
	*/
}

