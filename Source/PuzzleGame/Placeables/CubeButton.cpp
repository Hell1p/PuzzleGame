// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeButton.h"
#include "Components/BoxComponent.h"

ACubeButton::ACubeButton()
{
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ButtonMesh"));
	SetRootComponent(ButtonMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

void ACubeButton::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACubeButton::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ACubeButton::OnBoxEndOverlap);
}

void ACubeButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeButton::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bCubePlaced = true;
}

void ACubeButton::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCubePlaced = false;
}



