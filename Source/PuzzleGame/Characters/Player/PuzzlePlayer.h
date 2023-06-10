#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzlePlayer.generated.h"

class UCameraComponent;
class UPhysicsHandleComponent;

UCLASS()
class PUZZLEGAME_API APuzzlePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	APuzzlePlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void GrabButtonPressed();
	void GrabButtonReleased();

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere)
	float GrabDistance = 150.f;

	bool bGrabbingObject;
};
