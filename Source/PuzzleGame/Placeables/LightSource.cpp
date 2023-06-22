// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSource.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "PuzzleGame/Characters/Player/PuzzlePlayer.h"

ALightSource::ALightSource()
{
	PrimaryActorTick.bCanEverTick = true;

	LightSourceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LightSourceMesh"));
	RootComponent = LightSourceMesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(LightSourceMesh);
}

void ALightSource::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightSource::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ALightSource::OnSphereEndOverlap);
}

void ALightSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightSource::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APuzzlePlayer>(OtherActor);
	if (Player)
	{
		Player->OnLightOverlap();
	}
}

void ALightSource::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Player == nullptr ? Cast<APuzzlePlayer>(OtherActor) : Player;
	if (Player) Player->OnLightEndOverlap();
}