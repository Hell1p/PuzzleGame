
#include "Screwdriver.h"

#include "Components/SphereComponent.h"

AScrewdriver::AScrewdriver()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewDriverMesh"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	StaticMesh->SetupAttachment(GetRootComponent());
	SphereComponent->SetupAttachment(StaticMesh);
}


void AScrewdriver::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScrewdriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

