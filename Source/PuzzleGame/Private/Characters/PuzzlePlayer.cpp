#include "Characters/PuzzlePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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
	DirectionalMovement();
}

void APuzzlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APuzzlePlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APuzzlePlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APuzzlePlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APuzzlePlayer::Turn);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APuzzlePlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APuzzlePlayer::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APuzzlePlayer::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APuzzlePlayer::CrouchStart);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APuzzlePlayer::CrouchEnd);
}

void APuzzlePlayer::DirectionalMovement()
{
	FVector Velocity = GetVelocity();
	FRotator Rotation = GetActorRotation();
	float Direction = GetMesh()->GetAnimInstance()->CalculateDirection(Velocity, Rotation);
	
	if (Direction >= -50.f && Direction <= 50.f && !bSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	if ((Direction <= 90.f && Direction > 50.f) || (Direction >= -90.f && Direction < -50.f) && !bSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	if ((Direction < -90.f && Direction >= -180.f) || (Direction > 90.f && Direction <= 180.f))
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}
void APuzzlePlayer::MoveForward(float Value)
{
	if (Controller &&(Value != 0 ))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);
		const FVector Direction  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
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

void APuzzlePlayer::SprintStart()
{
	bSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}
void APuzzlePlayer::SprintEnd()
{
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void APuzzlePlayer::CrouchStart()
{
	Crouch();
}
void APuzzlePlayer::CrouchEnd()
{
	UnCrouch();
}
