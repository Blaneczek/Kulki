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

void UKulkiAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{		
	if (Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStrength());
		UE_LOG(LogTemp, Warning, TEXT("clamp strength"));
	}
	if (Attribute == GetSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSpeed());
	}
	
	Super::PreAttributeChange(Attribute, NewValue);
}

void UKulkiAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps) const
{
	OutProps.EffectContextHandle = Data.EffectSpec.GetContext();
	OutProps.SourceASC = OutProps.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(OutProps.SourceASC) && OutProps.SourceASC->AbilityActorInfo.IsValid() && IsValid(OutProps.SourceASC->GetAvatarActor()))
	{
		OutProps.SourceAvatarActor = OutProps.SourceASC->GetAvatarActor();
		OutProps.SourceController = OutProps.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (!OutProps.SourceController && OutProps.SourceAvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(OutProps.SourceAvatarActor))
			{
				OutProps.SourceController = Pawn->GetController();
			}
		}
		if (OutProps.SourceController)
		{
			OutProps.SourcePawn = OutProps.SourceController->GetPawn();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProps.TargetAvatarActor = Data.Target.GetAvatarActor();
		OutProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		OutProps.TargetPawn = Cast<APawn>(OutProps.TargetAvatarActor);
		OutProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProps.TargetAvatarActor);
	}
}

