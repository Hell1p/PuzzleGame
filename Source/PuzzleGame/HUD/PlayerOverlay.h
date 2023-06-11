// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UImage;
class UWidgetAnimation;

UCLASS()
class PUZZLEGAME_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* InteractCrosshair;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractCrosshairShow;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractCrosshairHide;
};
