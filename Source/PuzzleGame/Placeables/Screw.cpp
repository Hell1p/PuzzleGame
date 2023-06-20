// Fill out your copyright notice in the Description page of Project Settings.


#include "Screw.h"
#include "Ventilation.h"

AScrew::AScrew()
{
	PrimaryActorTick.bCanEverTick = true;

	ScrewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewMesh"));
	ScrewMesh->SetupAttachment(RootComponent);
	ScrewMesh->SetSimulatePhysics(false);
}

void AScrew::BeginPlay()
{
	Super::BeginPlay();
}

void AScrew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScrew::Unscrew()
{
	ScrewMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ScrewMesh->SetSimulatePhysics(true);
	AVentilation* Ventilation = Cast<AVentilation>(GetOwner());
	if (Ventilation == nullptr) return;
	Ventilation->OnUnscrewed();
}
