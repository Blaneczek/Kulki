// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/KulkiAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/KulkiGameplayAbility.h"


UKulkiAbilitySystemComponent::UKulkiAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UKulkiAbilitySystemComponent::AddCharactersAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UKulkiGameplayAbility* KulkiAbility = Cast<UKulkiGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(KulkiAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UKulkiAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UKulkiAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


