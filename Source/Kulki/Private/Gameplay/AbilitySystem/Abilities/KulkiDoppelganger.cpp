// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/AbilitySystem/Abilities/KulkiDoppelganger.h"

void UKulkiDoppelganger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("TEXTtttt"));
}
