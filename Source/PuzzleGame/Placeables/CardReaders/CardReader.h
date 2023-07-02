// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardReader.generated.h"

class UBoxComponent;
class UPointLightComponent;
class ADoor;
class ABaseCard;

UCLASS()
class PUZZLEGAME_API ACardReader : public AActor
{
	GENERATED_BODY()
	
public:	
	ACardReader();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int CardReaderLevel;

	UPROPERTY(BlueprintReadOnly)
	ABaseCard* Card;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void Open_Close();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = DoorProperties)
	ADoor* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPointLightComponent* RedLight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPointLightComponent* GreenLight;

private:
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CardReaderMesh;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;
};
