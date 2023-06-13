#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Screwdriver.generated.h"

class USphereComponent;

UCLASS()
class PUZZLEGAME_API AScrewdriver : public AActor
{
	GENERATED_BODY()
	
public:	
	AScrewdriver();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	
};
