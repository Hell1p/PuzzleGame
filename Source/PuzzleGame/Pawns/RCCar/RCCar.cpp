#include "RCCar.h"
#include "PuzzleGame/Characters/Player/PuzzlePlayer.h"
#include "Camera/CameraComponent.h"
#include "PuzzleGame/PlayerController/PuzzlePlayerController.h"
#include "ChaosWheeledVehicleMovementComponent.h"

ARCCar::ARCCar()
{
	PrimaryActorTick.bCanEverTick = true;

	RCCarCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RCCarCamera"));
	RCCarCamera->SetupAttachment(RootComponent);

	RCControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RCController"));
	RCControllerMesh->SetVisibility(false);
}

void ARCCar::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle AttachMeshTimer;
	GetWorldTimerManager().SetTimer(AttachMeshTimer, this, &ARCCar::AttachMeshToPuzzlePlayer, 0.3f);
 
	RCControllerMesh->SetVisibility(true);
}

void ARCCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARCCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("RCCar_Throttle"), this, &ARCCar::Throttle);
	PlayerInputComponent->BindAxis(FName("RCCar_Brake"), this, &ARCCar::Brake);
	PlayerInputComponent->BindAxis(FName("RCCar_Steering"), this, &ARCCar::Steering);

	PlayerInputComponent->BindAction(FName("RCCar_Handbrake"), IE_Pressed, this, &ARCCar::HandbrakeStart);
	PlayerInputComponent->BindAction(FName("RCCar_Handbrake"), IE_Released, this, &ARCCar::HandbrakeEnd);
	PlayerInputComponent->BindAction(FName("RCCar_Deactivate"), IE_Pressed, this, &ARCCar::Deactivate);
}

void ARCCar::Throttle(float Value)
{
	GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void ARCCar::Brake(float Value)
{
	GetVehicleMovementComponent()->SetBrakeInput(Value);
}

void ARCCar::Steering(float Value)
{
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void ARCCar::HandbrakeStart()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ARCCar::HandbrakeEnd()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ARCCar::AttachMeshToPuzzlePlayer()
{
	APuzzlePlayer* PuzzlePlayer = Cast<APuzzlePlayer>(GetOwner());
	if (PuzzlePlayer) RCControllerMesh->AttachToComponent(PuzzlePlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("S_RCController"));
}

void ARCCar::Deactivate()
{
	if (RCCarState == ERCCarState::ERCCS_Possessed && ControllingPlayer)
	{
		RCCarState = ERCCarState::ERCCS_Initial;
		ControllingPlayer->GetPuzzlePlayerController()->Possess(ControllingPlayer);
	}
}

void ARCCar::Activate(APuzzlePlayer* Player, APuzzlePlayerController* CharacterController)
{
	if (Player == nullptr || CharacterController == nullptr) return;
	
	if (RCCarState == ERCCarState::ERCCS_Initial)
	{
		RCCarState = ERCCarState::ERCCS_Possessed;
		ControllingPlayer = Player;
		CharacterController->Possess(this);
	}
}
