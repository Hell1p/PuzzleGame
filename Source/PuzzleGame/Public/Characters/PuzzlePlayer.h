#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PuzzlePlayer.generated.h"

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

private:
	bool bSprinting;
	void DirectionalMovement();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void SprintStart();
	void SprintEnd();
	void CrouchStart();
	void CrouchEnd();
	
};
