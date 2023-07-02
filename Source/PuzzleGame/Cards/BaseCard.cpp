// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCard.h"

ABaseCard::ABaseCard()
{
	PrimaryActorTick.bCanEverTick = false;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardMesh"));
	CardMesh->SetupAttachment(RootComponent);
}

void ABaseCard::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

