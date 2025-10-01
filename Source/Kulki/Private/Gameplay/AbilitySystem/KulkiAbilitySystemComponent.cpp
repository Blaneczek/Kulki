// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/AbilitySystem/KulkiAbilitySystemComponent.h"
#include "Gameplay/AbilitySystem/Abilities/KulkiGameplayAbility.h"


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
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(KulkiAbility->InputTag);
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


