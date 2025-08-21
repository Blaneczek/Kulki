// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiWidgetController.h"
#include "Character/KulkiPlayerCharacter.h"

void UKulkiWidgetController::SetWidgetControllerParams(AKulkiPlayerCharacter* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void UKulkiWidgetController::InitAttributesValue()
{
	if (IsValid(PlayerCharacter))
	{
		OnStrengthChanged.Broadcast(PlayerCharacter->GetStrengthValue());
		OnSpeedChanged.Broadcast(PlayerCharacter->GetSpeedValue());		
	}	
}

void UKulkiWidgetController::BindCallbacks()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnStrengthChanged.AddLambda(
			[this](float NewStrength)
		{
			OnStrengthChanged.Broadcast(NewStrength);
		});

		PlayerCharacter->OnSpeedChanged.AddLambda(
			[this](float NewSpeed)
		{
			OnSpeedChanged.Broadcast(NewSpeed);
		});
	}
}
