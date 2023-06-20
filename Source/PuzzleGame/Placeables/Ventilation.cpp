// Fill out your copyright notice in the Description page of Project Settings.


#include "Ventilation.h"
#include "Screw.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AVentilation::AVentilation()
{
	PrimaryActorTick.bCanEverTick = true;

	VentilationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VentilationMesh"));
	VentilationMesh->SetupAttachment(RootComponent);

	
}

void AVentilation::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < 4; i++)
	{
		AScrew* Screw = GetWorld()->SpawnActor<AScrew>(ScrewClass);
		if (Screw == nullptr) return;
		Screw->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, FName("S_Screw" + FString::FromInt(i)));
		Screw->Owner = this;
	}
}

void AVentilation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVentilation::OnUnscrewed()
{
	VentilationMesh->SetSimulatePhysics(true);
}
