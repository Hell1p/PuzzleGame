#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Screwdriver.generated.h"

class UBoxComponent;

UCLASS()
class PUZZLEGAME_API AScrewdriver : public AActor
{
	GENERATED_BODY()
	
public:	
	AScrewdriver();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ScrewdriverMesh;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;
};
