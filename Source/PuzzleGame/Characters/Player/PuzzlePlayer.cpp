#include "PuzzlePlayer.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"

APuzzlePlayer::APuzzlePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void APuzzlePlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APuzzlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bGrabbingObject)
	{
		PhysicsHandle->SetTargetLocation(PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation());
	}
}

void APuzzlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APuzzlePlayer::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APuzzlePlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APuzzlePlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APuzzlePlayer::Turn);
	
	PlayerInputComponent->BindAction(TEXT("GrabRequest"), IE_Pressed, this, &APuzzlePlayer::GrabButtonPressed);
	PlayerInputComponent->BindAction(TEXT("GrabRequest"), IE_Released, this, &APuzzlePlayer::GrabButtonReleased);
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

void APuzzlePlayer::GrabButtonPressed()
{
	if (GetWorld() == nullptr) return;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility);
	if (!HitResult.bBlockingHit) return;
	PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), HitResult.BoneName, HitResult.GetComponent()->GetComponentLocation(), HitResult.GetComponent()->GetComponentRotation());
	bGrabbingObject = true;
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation(), FColor::Cyan, true, 2.f, 2, 2);
}

void APuzzlePlayer::GrabButtonReleased()
{
	if (bGrabbingObject)
	{
		PhysicsHandle->ReleaseComponent();
		bGrabbingObject = false;
	}
}

