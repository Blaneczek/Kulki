// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"
#include "Gameplay/KulkiGameplayTags.h"

UKulkiAttributeSet::UKulkiAttributeSet()
{
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



