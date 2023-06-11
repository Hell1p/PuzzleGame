// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleHUD.h"
#include "PlayerOverlay.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

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

void APuzzleHUD::HideInteractCrosshair()
{
	/*if (PlayerOverlay)
	{
		PlayerOverlay->InteractCrosshair->SetVisibility(ESlateVisibility::Hidden);
	}*/
	if (PlayerOverlay)
	{
		PlayerOverlay->PlayAnimation(PlayerOverlay->InteractCrosshairShow);
	}
}

void APuzzleHUD::ShowInteractCrosshair()
{
	/*if (PlayerOverlay)
	{
		PlayerOverlay->InteractCrosshair->SetVisibility(ESlateVisibility::Visible);
	}*/
	if (PlayerOverlay)
	{
		PlayerOverlay->PlayAnimation(PlayerOverlay->InteractCrosshairHide);
	}
}
