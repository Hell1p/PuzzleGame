// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh);

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetupAttachment(RootComponent);

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	LeftDoorMesh->SetupAttachment(FrameMesh);

	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
	RightDoorMesh->SetupAttachment(FrameMesh);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Button && Button->GetbCubePlaced())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, TEXT("DoorOpens"));
	}
}
