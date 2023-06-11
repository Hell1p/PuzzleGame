// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PuzzleHUD.generated.h"

class UPlayerOverlay;
class UUserWidget;

UCLASS()
class PUZZLEGAME_API APuzzleHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = PlayerStats)
	TSubclassOf<UUserWidget> PlayerOverlayClass;
	
	UPlayerOverlay* PlayerOverlay;
	
	void HideInteractCrosshair();
	void ShowInteractCrosshair();

protected:
	virtual void BeginPlay() override;
	void AddPlayerOverlay();
};
