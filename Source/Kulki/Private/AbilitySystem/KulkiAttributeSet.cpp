// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/KulkiAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Gameplay/KulkiGameplayTags.h"

UKulkiAttributeSet::UKulkiAttributeSet()
{
	TagsToAttributes.Add(KulkiGameplayTags::Attribute_Strength.GetTag(), GetStrengthAttribute);
	TagsToAttributes.Add(KulkiGameplayTags::Attribute_MaxStrength.GetTag(), GetMaxStrengthAttribute);
	TagsToAttributes.Add(KulkiGameplayTags::Attribute_Speed.GetTag(), GetSpeedAttribute);
	TagsToAttributes.Add(KulkiGameplayTags::Attribute_MaxSpeed.GetTag(), GetMaxSpeedAttribute);
}

void UKulkiAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{		
	if (Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStrength());	
	}
	if (Attribute == GetSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSpeed());
	}
	
	if (NewValue <= 0.f)
    {
    	OnAttributeReachedZero.Broadcast();
    }
	
	Super::PreAttributeBaseChange(Attribute, NewValue);
}



