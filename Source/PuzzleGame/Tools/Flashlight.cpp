// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AFlashlight::AFlashlight()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

void AFlashlight::LightTurnOff(bool PlaySound)
{
	SpotLight->SetVisibility(false);
	
	if (PlaySound) UGameplayStatics::PlaySoundAtLocation(this, FlashlightOffSound, GetActorLocation(), GetActorRotation());
}

void AFlashlight::LightTurnOn(bool PlaySound)
{
	SpotLight->SetVisibility(true);
	if (PlaySound) UGameplayStatics::PlaySoundAtLocation(this, FlashlightOnSound, GetActorLocation(), GetActorRotation());
}