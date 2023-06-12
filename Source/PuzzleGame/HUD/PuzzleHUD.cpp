// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleHUD.h"
#include "PlayerOverlay.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

void APuzzleHUD::BeginPlay()
{
	Super::BeginPlay();
	AddPlayerOverlay();
}

void APuzzleHUD::DrawHUD()
{
	Super::DrawHUD();
}

void APuzzleHUD::AddPlayerOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && PlayerOverlayClass)
	{
		PlayerOverlay = CreateWidget<UPlayerOverlay>(PlayerController, PlayerOverlayClass);
		PlayerOverlay->AddToViewport();
	}
}

void APuzzleHUD::SetStaminaBarPercent(float Percent)
{
	if (PlayerOverlay) PlayerOverlay->StaminaBar->SetPercent(Percent);
}

void APuzzleHUD::HideInteractCrosshair()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->PlayAnimation(PlayerOverlay->InteractCrosshairShow);
	}
}

void APuzzleHUD::ShowInteractCrosshair()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->PlayAnimation(PlayerOverlay->InteractCrosshairHide);
	}
}

void APuzzleHUD::HideStaminaBar()
{
	if (PlayerOverlay) PlayerOverlay->PlayAnimation(PlayerOverlay->StaminaBarHide);
}

void APuzzleHUD::ShowStaminaBar()
{
	if (PlayerOverlay) PlayerOverlay->PlayAnimation(PlayerOverlay->StaminaBarShow);
}
