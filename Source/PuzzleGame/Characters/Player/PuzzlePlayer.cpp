#include "PuzzlePlayer.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"
#include "PuzzleGame/HUD/PlayerOverlay.h"
#include "PuzzleGame/HUD/PuzzleHUD.h"
#include "PuzzleGame/Tools/Flashlight.h"

APuzzlePlayer::APuzzlePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(PlayerCamera);
	
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void APuzzlePlayer::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh()) GetMesh()->SetVisibility(false);

	InitializeInventoryTools();
	InitializePuzzleOverlay();
	CurrentStamina = MaxStamina;
	if (HUD) HUD->GetPlayerOverlay()->StaminaBar->SetVisibility(ESlateVisibility::Hidden);
	StaminaBarHidden = true;
}

void APuzzlePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bGrabbingObject)
	{
		PhysicsHandle->SetTargetLocation(PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation());
	}

	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	if (bSprinting && CurrentStamina >= SprintCost && Velocity.Size() > 0.f)
	{
		UseStamina(SprintCost);
		if (HUD) HUD->SetStaminaBarPercent(CurrentStamina / MaxStamina);
	}
	if (bSprinting && CurrentStamina < SprintCost) bSprinting = false;
	if (!bSprinting) RegenerateStamina(DeltaTime);

	if (CurrentStamina == MaxStamina && !StaminaBarHidden)
	{
		StaminaBarHide();
	}
	
	StartSprintingWhenNeeded();
	DirectionalMovement();
	SetSprintingFOV(DeltaTime);
	SetGrabbedObjectLocation();
	InteractCrosshair();
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
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APuzzlePlayer::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APuzzlePlayer::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("On_OffFlashLight"), IE_Pressed, this, &APuzzlePlayer::FlashlightOn_Off);
}

void APuzzlePlayer::MoveForward(float Value)
{
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);
		const FVector Direction  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void APuzzlePlayer::MoveRight(float Value)
{
	if (Controller && (Value != 0))
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
	if (CurrentStamina < SprintCost) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	
	if (!StaminaBarHidden)
	{
		StaminaBarHidden = false;
	}
	else if (Velocity.Size() > 0.f) StaminaBarShow();

	if (bCrouching) bWantsToSprint = true;
	if (PlayerMovementDirectionState == EMovementDirectionState::EMDS_Backward) bWantsToSprint = true;
	if (!bCrouching)
	{
		bSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
}
void APuzzlePlayer::SprintEnd()
{
	bWantsToSprint = false;
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

void APuzzlePlayer::FlashlightOn_Off()
{
	if (Flashlight == nullptr) return;
	if (!Flashlight->GetbFlashlightTurnedOn())
	{
		Flashlight->LightTurnOn();
		Flashlight->SetbFlashlightTurnedOn(true);
	}
	else
	{
		Flashlight->LightTurnOff();
		Flashlight->SetbFlashlightTurnedOn(false);
	}
}

void APuzzlePlayer::SetSprintingFOV(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	if (!bCrouching && bSprinting && Velocity.Size() > 0.f)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, SprintFOV, DeltaTime, 2.f);
	}
	else
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
}

void APuzzlePlayer::GrabButtonReleased()
{
	if (bGrabbingObject)
	{
		PhysicsHandle->ReleaseComponent();
		bGrabbingObject = false;
	}
}

void APuzzlePlayer::SetGrabbedObjectLocation()
{
	if (bGrabbingObject)
	{
		PhysicsHandle->SetTargetLocation(PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation());
	}
}

void APuzzlePlayer::DirectionalMovement()
{
	FVector Velocity = GetVelocity();
	FRotator Rotation = GetActorRotation();
	float Direction = GetMesh()->GetAnimInstance()->CalculateDirection(Velocity, Rotation);
	
	if (Direction >= -50.f && Direction <= 50.f && !bSprinting && !bCrouching)
	{
		PlayerMovementDirectionState = EMovementDirectionState::EMDS_Forward;
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	}
	if ((Direction <= 90.f && Direction > 50.f) || (Direction >= -90.f && Direction < -50.f) && !bSprinting && !bCrouching)
	{
		PlayerMovementDirectionState = EMovementDirectionState::EMDS_Left_Right;
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedL_R;
	}
	if ((Direction < -90.f && Direction >= -180.f) || (Direction > 90.f && Direction <= 180.f))
	{
		if (bSprinting)
		{
			bWantsToSprint = true;
			bSprinting = false;
		}
		PlayerMovementDirectionState = EMovementDirectionState::EMDS_Backward;
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedBwd;
	}
}

void APuzzlePlayer::StartSprintingWhenNeeded()
{
	if ((!bCrouching && bWantsToSprint) || (PlayerMovementDirectionState != EMovementDirectionState::EMDS_Backward && bWantsToSprint)) SprintStart();
}

void APuzzlePlayer::InteractCrosshair()
{
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

void APuzzlePlayer::UseStamina(float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0.f, MaxStamina);
}

void APuzzlePlayer::InitializePuzzleOverlay()
{
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		HUD = Cast<APuzzleHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			PlayerOverlay = HUD->GetPlayerOverlay();
		}
	}
}

void APuzzlePlayer::RegenerateStamina(float DeltaTime)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
	if (HUD) HUD->SetStaminaBarPercent(CurrentStamina / MaxStamina);
}

void APuzzlePlayer::StaminaBarHide()
{
	if (HUD)
	{
		HUD->HideStaminaBar();
		StaminaBarHidden = true;
	}
}

void APuzzlePlayer::StaminaBarShow()
{
	if (HUD)
	{
		HUD->ShowStaminaBar();
		StaminaBarHidden = false;
	}
}

void APuzzlePlayer::InitializeInventoryTools()
{
	if (GetWorld() == nullptr) return;
	AFlashlight* FlashlightToEquip = GetWorld()->SpawnActor<AFlashlight>(FlashlightClass);
	if (FlashlightToEquip == nullptr) return;
	Flashlight = FlashlightToEquip;
	Flashlight->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("S_Flashlight"));
}
