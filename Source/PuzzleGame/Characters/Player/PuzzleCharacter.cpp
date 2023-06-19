#include "PuzzleCharacter.h"
#include "KismetAnimationLibrary.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"
#include "PuzzleGame/HUD/PlayerOverlay.h"
#include "PuzzleGame/HUD/PuzzleHUD.h"
#include "PuzzleGame/Pawns/RCCar/RCCar.h"
#include "PuzzleGame/Tools/Flashlight.h"
#include "PuzzleGame/PlayerController/PuzzlePlayerController.h"
APuzzleCharacter::APuzzleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	GetMesh()->SetupAttachment(PlayerCamera);
	
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

void APuzzleCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh()) GetMesh()->SetVisibility(false);

	InitializeInventoryTools();
	InitializePuzzleOverlay();
	CurrentStamina = MaxStamina;
	if (HUD) HUD->GetPlayerOverlay()->StaminaBar->SetVisibility(ESlateVisibility::Hidden);
	StaminaBarHidden = true;
}

void APuzzleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bGrabbingObject)
	{
		PhysicsHandle->SetTargetLocation(PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation());
	}
	DirectionalMovement();
	StartSprintingWhenNeeded();
	HandleStamina(DeltaTime);
	SetSprintingFOV(DeltaTime);
	SetGrabbedObjectLocation();
	InteractCrosshair();
}

void APuzzleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APuzzleCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APuzzleCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APuzzleCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APuzzleCharacter::Turn);

	PlayerInputComponent->BindAxis(TEXT("RotateGrabbedObject_Right"), this, &APuzzleCharacter::RotateGrabbedObject_Right);
	PlayerInputComponent->BindAxis(TEXT("RotateGrabbedObject_Left"), this, &APuzzleCharacter::RotateGrabbedObject_Left);

	PlayerInputComponent->BindAction(TEXT("GrabRequest"), IE_Pressed, this, &APuzzleCharacter::GrabButtonPressed);
	PlayerInputComponent->BindAction(TEXT("GrabRequest"), IE_Released, this, &APuzzleCharacter::GrabButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APuzzleCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APuzzleCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APuzzleCharacter::SprintEnd);
	PlayerInputComponent->BindAction(TEXT("On_OffFlashLight"), IE_Pressed, this, &APuzzleCharacter::FlashlightOn_Off);
	PlayerInputComponent->BindAction(TEXT("ToggleRCCar"), IE_Pressed, this, &APuzzleCharacter::ToggleRCCar);

	PlayerInputComponent->BindAction(TEXT("SlotSwitch_1"), IE_Pressed, this, &APuzzleCharacter::SlotSwitch_1);
	PlayerInputComponent->BindAction(TEXT("SlotSwitch_2"), IE_Pressed, this, &APuzzleCharacter::SlotSwitch_2);
}

void APuzzleCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);
		const FVector Direction  = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
	}
}

void APuzzleCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
	}
}

void APuzzleCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APuzzleCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
void APuzzleCharacter::SprintStart()
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
	if (PlayerMovementDirectionState == EMovementDirectionState1::EMDS_Backward) bWantsToSprint = true;
	if (!bCrouching)
	{
		bSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
}
void APuzzleCharacter::SprintEnd()
{
	bWantsToSprint = false;
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

void APuzzleCharacter::FlashlightOn_Off()
{
	if (Flashlight == nullptr || PlayerToolEquippedState != EPlayerToolEquippedState1::EPTES_Flashlight) return;
	
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

void APuzzleCharacter::SetSprintingFOV(float DeltaTime)
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

void APuzzleCharacter::GrabButtonPressed()
{
	if (GetWorld() == nullptr) return;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility);
	if (!HitResult.bBlockingHit) return;
	PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), HitResult.BoneName, HitResult.GetComponent()->GetComponentLocation(), HitResult.GetComponent()->GetComponentRotation());
	bGrabbingObject = true;
}

void APuzzleCharacter::GrabButtonReleased()
{
	if (bGrabbingObject)
	{
		PhysicsHandle->ReleaseComponent();
		bGrabbingObject = false;
	}
}

void APuzzleCharacter::SetGrabbedObjectLocation()
{
	if (bGrabbingObject)
	{
		PhysicsHandle->SetTargetLocation(PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation());
	}
}

void APuzzleCharacter::DirectionalMovement()
{
	FVector Velocity = GetVelocity();
	FRotator Rotation = GetActorRotation();
	
	FMatrix rotationMatrix = FRotationMatrix(Rotation);
	//FVector forwardVector = rotationMatrix.GetUnitAxis(EAxis::X);
	//float Direction = FVector::DotProduct(Velocity.GetSafeNormal(), forwardVector);
	
	const float Direction = GetMesh()->GetAnimInstance()->CalculateDirection(Velocity, Rotation);
    //float Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Rotation);
	if (Direction >= -50.f && Direction <= 50.f && !bSprinting && !bCrouching)
	{
		PlayerMovementDirectionState = EMovementDirectionState1::EMDS_Forward;
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	}
	if ((Direction <= 90.f && Direction > 50.f) || (Direction >= -90.f && Direction < -50.f) && !bSprinting && !bCrouching)
	{
		PlayerMovementDirectionState = EMovementDirectionState1::EMDS_Left_Right;
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedL_R;
	}
	if ((Direction < -90.f && Direction >= -180.f) || (Direction > 90.f && Direction <= 180.f))
	{
		if (bSprinting)
		{
			bWantsToSprint = true;
			bSprinting = false;
		}
		PlayerMovementDirectionState = EMovementDirectionState1::EMDS_Backward;
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeedBwd;
	}
}

void APuzzleCharacter::StartSprintingWhenNeeded()
{
	if ((!bCrouching && bWantsToSprint) || (PlayerMovementDirectionState != EMovementDirectionState1::EMDS_Backward && bWantsToSprint)) SprintStart();
}

void APuzzleCharacter::InteractCrosshair()
{
	PlayerController = PlayerController == nullptr ? Cast<APlayerController>(Controller) : PlayerController;
	if (PlayerController) HUD = HUD == nullptr ? Cast<APuzzleHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD == nullptr) return;

	FHitResult HitResult;
	if (GetWorld()) return;
	GetWorld()->LineTraceSingleByChannel(HitResult, PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * GrabDistance + PlayerCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility);
	if (!HitResult.bBlockingHit) HUD->HideInteractCrosshair();
	if (!HitResult.bBlockingHit) return;
	if (HitResult.GetActor()->GetRootComponent()->IsSimulatingPhysics()) HUD->ShowInteractCrosshair();
	else HUD->HideInteractCrosshair();
}

void APuzzleCharacter::UseStamina(float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0.f, MaxStamina);
}

void APuzzleCharacter::InitializePuzzleOverlay()
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
void APuzzleCharacter::Jump()
{
	if (!bBlockingHitR)
	{
		Super::Jump();
	}

}

void APuzzleCharacter::RegenerateStamina(float DeltaTime)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
	if (HUD) HUD->SetStaminaBarPercent(CurrentStamina / MaxStamina);
}

void APuzzleCharacter::StaminaBarHide()
{
	if (HUD)
	{
		HUD->HideStaminaBar();
		StaminaBarHidden = true;
	}
}

void APuzzleCharacter::StaminaBarShow()
{
	if (HUD)
	{
		HUD->ShowStaminaBar();
		StaminaBarHidden = false;
	}
}

void APuzzleCharacter::InitializeInventoryTools()
{
	if (GetWorld() == nullptr) return;
	
	/** Flashlight */
	if (FlashlightClass == nullptr) return;
	AFlashlight* FlashlightToEquip = GetWorld()->SpawnActor<AFlashlight>(FlashlightClass);
	if (FlashlightToEquip == nullptr) return;
	Flashlight = FlashlightToEquip;
	Flashlight->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("S_Flashlight"));

	/** RCCar */ //сасі
	if (RCCarClass == nullptr) return;
	ARCCar* RCCarToEquip = GetWorld()->SpawnActor<ARCCar>(RCCarClass);
	if (RCCarToEquip == nullptr) return;
	RCCar = RCCarToEquip;
	RCCar->SetOwner(this);
	RCCar->GetRCControllerMesh()->SetVisibility(false);
}

void APuzzleCharacter::HandleStamina(float DeltaTime)
{
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
}

void APuzzleCharacter::ToggleRCCar()
{
	if (RCCar == nullptr || PlayerToolEquippedState != EPlayerToolEquippedState1::EPTES_RCCar) return;
	
	PuzzlePlayerController = PuzzlePlayerController == nullptr ? Cast<APuzzlePlayerController>(Controller) : PuzzlePlayerController;
	if (PuzzlePlayerController == nullptr) return;
	GetMovementComponent()->StopMovementImmediately();
	//RCCar->Activate(this, PuzzlePlayerController);
}

APuzzlePlayerController* APuzzleCharacter::GetPuzzlePlayerController()
{
	PuzzlePlayerController = PuzzlePlayerController == nullptr ? Cast<APuzzlePlayerController>(Controller) : PuzzlePlayerController;
	return PuzzlePlayerController;
}

void APuzzleCharacter::RotateGrabbedObject_Right(float Value)
{
	if (Value == 0.f) return;
	FRotator PrevRotation;
	FVector PrevLocation;
	PhysicsHandle->GetTargetLocationAndRotation(PrevLocation, PrevRotation);
	PhysicsHandle->SetTargetRotation(FRotator(PrevRotation.Pitch, ++PrevRotation.Yaw, PrevRotation.Roll));
}

void APuzzleCharacter::RotateGrabbedObject_Left(float Value)
{
	if (Value == 0.f) return;
	FRotator PrevRotation;
	FVector PrevLocation;
	PhysicsHandle->GetTargetLocationAndRotation(PrevLocation, PrevRotation);
	PhysicsHandle->SetTargetRotation(FRotator(PrevRotation.Pitch, --PrevRotation.Yaw, PrevRotation.Roll));
}

void APuzzleCharacter::SlotSwitch_1() // Flashlight
{
	if (Flashlight == nullptr) return;

	if (RCCar && PlayerToolEquippedState == EPlayerToolEquippedState1::EPTES_RCCar)
	{
		RCCar->GetRCControllerMesh()->SetVisibility(false);
	}
	PlayerToolEquippedState = EPlayerToolEquippedState1::EPTES_Flashlight;
	Flashlight->GetFlashlightMesh()->SetVisibility(true);
	if (bPrevLightTurnedOn) Flashlight->LightTurnOn();
}

void APuzzleCharacter::SlotSwitch_2() // RCCar
{
	if (RCCar == nullptr) return;

	if (Flashlight && PlayerToolEquippedState == EPlayerToolEquippedState1::EPTES_Flashlight)
	{
		bPrevLightTurnedOn = Flashlight->GetbFlashlightTurnedOn();
		Flashlight->LightTurnOff();
		Flashlight->GetFlashlightMesh()->SetVisibility(false);
	}
	PlayerToolEquippedState = EPlayerToolEquippedState1::EPTES_RCCar;
	RCCar->GetRCControllerMesh()->SetVisibility(true);
}

