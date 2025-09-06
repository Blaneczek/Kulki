// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/KulkiAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Gameplay/KulkiGameplayTags.h"

UKulkiAttributeSet::UKulkiAttributeSet()
{
	const FKulkiGameplayTags& GameplayTags = FKulkiGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attribute_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_MaxStrength, GetMaxStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Speed, GetSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_MaxSpeed, GetMaxSpeedAttribute);
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

void UKulkiAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetStrengthAttribute())
	{
		SetStrength(FMath::Clamp(GetStrength(), 0.0f, GetMaxStrength()));
	}
	if (Data.EvaluatedData.Attribute == GetSpeedAttribute())
	{
		SetSpeed(FMath::Clamp(GetSpeed(), 0.0f, GetMaxSpeed()));
	}
}

