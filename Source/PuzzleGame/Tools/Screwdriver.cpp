
#include "Screwdriver.h"
#include "Components/BoxComponent.h"
#include "PuzzleGame/Placeables/Screw.h"

AScrewdriver::AScrewdriver()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ScrewdriverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScrewdriverMesh"));
	ScrewdriverMesh->SetupAttachment(GetRootComponent());
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereCollision"));
	BoxCollision->SetupAttachment(ScrewdriverMesh);
}

void AScrewdriver::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AScrewdriver::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AScrewdriver::OnBoxEndOverlap);
}

void AScrewdriver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScrewdriver::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AScrew* Screw = Cast<AScrew>(OtherActor);
	if (Screw)
	{
		Screw->Unscrew();
	}
}

void AScrewdriver::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
