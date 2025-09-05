// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiWidgetController.h"
#include "Character/KulkiPlayerPawn.h"


void UKulkiWidgetController::SetWidgetControllerParams(AKulkiPlayerPawn* InPlayerPawn)
{
	PlayerPawn = InPlayerPawn;
}

void UKulkiWidgetController::InitAttributesValue()
{
	if (IsValid(PlayerPawn))
	{
		OnMaxStrengthChanged.Broadcast(PlayerPawn->GetAttributesComponent()->StrengthAttribute.MaxValue);
     	OnMaxSpeedChanged.Broadcast(PlayerPawn->GetAttributesComponent()->SpeedAttribute.MaxValue);
		OnStrengthChanged.Broadcast(PlayerPawn->GetAttributesComponent()->StrengthAttribute.Value);
		OnSpeedChanged.Broadcast(PlayerPawn->GetAttributesComponent()->SpeedAttribute.Value);		
	}	
}

void UKulkiWidgetController::BindCallbacks()
{
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->GetAttributesComponent()->OnStrengthChangedDelegate.AddLambda(
			[this](float NewStrength)
		{
			OnStrengthChanged.Broadcast(NewStrength);
		});

		PlayerPawn->GetAttributesComponent()->OnSpeedChangedDelegate.AddLambda(
			[this](float NewSpeed)
		{
			OnSpeedChanged.Broadcast(NewSpeed);
		});
	}
}
