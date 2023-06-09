#include "Characters/PuzzlePlayer.h"

APuzzlePlayer::APuzzlePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzlePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}


void APuzzlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APuzzlePlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APuzzlePlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APuzzlePlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APuzzlePlayer::Turn);
	
}

void APuzzlePlayer::MoveForward(float Value)
{
	if (Controller &&(Value != 0 ))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);
		const FVector Direction  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void APuzzlePlayer::MoveRight(float Value)
{
	if (Controller &&(Value != 0 ))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
	}
}

void APuzzlePlayer::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APuzzlePlayer::Turn(float Value)
{
	AddControllerYawInput(Value);
}
