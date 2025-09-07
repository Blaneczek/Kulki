// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KulkiWidgetController.h"
#include "AbilitySystem/KulkiAttributeSet.h"


void UKulkiWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerPawn = WCParams.PlayerPawn;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UKulkiWidgetController::BroadcastInitialValues()
{
	const UKulkiAttributeSet* KulkiAttributeSet = CastChecked<UKulkiAttributeSet>(AttributeSet);
	
	OnMaxStrengthChanged.Broadcast(KulkiAttributeSet->GetMaxStrength());
    OnMaxSpeedChanged.Broadcast(KulkiAttributeSet->GetMaxSpeed());
	OnStrengthChanged.Broadcast(KulkiAttributeSet->GetStrength());
	OnSpeedChanged.Broadcast(KulkiAttributeSet->GetSpeed());		
}

void UKulkiWidgetController::BindCallbacks()
{
	const UKulkiAttributeSet* KulkiAttributeSet = CastChecked<UKulkiAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		KulkiAttributeSet->GetStrengthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStrengthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		KulkiAttributeSet->GetSpeedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnSpeedChanged.Broadcast(Data.NewValue);
		});
}
