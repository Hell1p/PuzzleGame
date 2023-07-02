// Fill out your copyright notice in the Description page of Project Settings.


#include "CardReader.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "PuzzleGame/Cards/BaseCard.h"

ACardReader::ACardReader()
{
	PrimaryActorTick.bCanEverTick = false;

	CardReaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardReaderMesh"));
	CardReaderMesh->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(CardReaderMesh);

	RedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RedLight"));
	RedLight->SetupAttachment(CardReaderMesh);
	GreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("GreenLight"));
	GreenLight->SetupAttachment(CardReaderMesh);
}

void ACardReader::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACardReader::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ACardReader::OnBoxEndOverlap);
}

void ACardReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACardReader::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Door == nullptr) return;
	Card = Cast<ABaseCard>(OtherActor);
	
	if (Card) Open_Close();
}

void ACardReader::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

