// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"

AFlashlight::AFlashlight()
{
	PrimaryActorTick.bCanEverTick = false;
	
	FlashlightMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FlashlightMesh"));
	FlashlightMesh->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(FlashlightMesh);
}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();
}

void AFlashlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlashlight::LightTurnOff()
{
	SpotLight->SetVisibility(false);
}

void AFlashlight::LightTurnOn()
{
	SpotLight->SetVisibility(true);
}