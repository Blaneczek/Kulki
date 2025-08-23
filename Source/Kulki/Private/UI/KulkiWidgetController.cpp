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
		OnStrengthChanged.Broadcast(PlayerCharacter->GetAttributesComponent()->StrengthAttribute.Value);
		OnSpeedChanged.Broadcast(PlayerCharacter->GetAttributesComponent()->StrengthAttribute.Value);		
	}	
}

void UKulkiWidgetController::BindCallbacks()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetAttributesComponent()->OnStrengthChangedDelegate.AddLambda(
			[this](float NewStrength)
		{
			OnStrengthChanged.Broadcast(NewStrength);
		});

		PlayerCharacter->GetAttributesComponent()->OnSpeedChangedDelegate.AddLambda(
			[this](float NewSpeed)
		{
			OnSpeedChanged.Broadcast(NewSpeed);
		});
	}
}
