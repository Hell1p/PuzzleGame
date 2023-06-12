#include "PuzzlePlayer.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PuzzleGame/HUD/PuzzleHUD.h"
#include "GameFramework/PlayerController.h"

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
	
	DirectionalMovement();
	SetSprintingFOV(DeltaTime);
	if (bGrabbingObject)
	{
		PhysicsHandle->SetTargetLocation(PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation());
	}
	
	PlayerController = PlayerController == nullptr ? Cast<APlayerController>(Controller) : PlayerController;
	if (PlayerController) HUD = HUD == nullptr ? Cast<APuzzleHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD == nullptr) return;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility);
	if (!HitResult.bBlockingHit) HUD->HideInteractCrosshair();
	if (!HitResult.bBlockingHit) return;
	if (HitResult.GetActor()->GetRootComponent()->IsSimulatingPhysics()) HUD->ShowInteractCrosshair();
	else HUD->HideInteractCrosshair();
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
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APuzzlePlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APuzzlePlayer::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APuzzlePlayer::SprintEnd);
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

void APuzzlePlayer::SprintStart()
{
	if (!bCrouching )
	{
		bSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	
}
void APuzzlePlayer::SprintEnd()
{
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void APuzzlePlayer::SetSprintingFOV(float DeltaTime)
{
		
	if (bSprinting && !bCrouching)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, SprintFOV, DeltaTime, 2.f);
		
	}else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, 10.f);
	}
	if (PlayerCamera)
	{
		PlayerCamera->SetFieldOfView(CurrentFOV);
	}
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

void APuzzlePlayer::DirectionalMovement()
{
	FVector Velocity = GetVelocity();
	FRotator Rotation = GetActorRotation();
	float Direction = GetMesh()->GetAnimInstance()->CalculateDirection(Velocity, Rotation);
	
	if (Direction >= -50.f && Direction <= 50.f && !bSprinting && !bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	}
	if ((Direction <= 90.f && Direction > 50.f) || (Direction >= -90.f && Direction < -50.f) && !bSprinting && !bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedL_R;
	}
	if ((Direction < -90.f && Direction >= -180.f) || (Direction > 90.f && Direction <= 180.f))
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedBwd;
	}
}


