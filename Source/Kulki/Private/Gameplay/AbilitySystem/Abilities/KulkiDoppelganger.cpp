// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Gameplay/AbilitySystem/Abilities/KulkiDoppelganger.h"
#include "Character/KulkiDoppelgangerPawn.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"
#include "Gameplay/AbilitySystem/AbilityTasks/TargetDataUnderCursor.h"
#include "Kismet/GameplayStatics.h"

bool UKulkiDoppelganger::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{	
	if (const UKulkiAttributeSet* KulkiAS = Cast<UKulkiAttributeSet>(ActorInfo->AbilitySystemComponent->GetAttributeSet(UKulkiAttributeSet::StaticClass())))		
	{
		// Minimum Strength to perform ability
		return KulkiAS->GetStrength() >= 20.f;			
	}
	
	return false;
}

void UKulkiDoppelganger::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UKulkiDoppelganger::SpawnDoppelganger(const FVector& TargetLocation)
{
	if (!DoppelgangerPawnClass || !DoppelgangerEffectClass)
	{
		return;
	}
	
	const FVector OwnerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Direction = (TargetLocation - OwnerLocation).GetSafeNormal2D();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(OwnerLocation);
	SpawnTransform.SetRotation(Direction.ToOrientationQuat());
	AKulkiDoppelgangerPawn* Doppelganger = GetWorld()->SpawnActorDeferred<AKulkiDoppelgangerPawn>(
		DoppelgangerPawnClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
                    				
	if (IsValid(Doppelganger))
	{
		Doppelganger->SetData(GetAvatarActorFromActorInfo(), MergeTime);
		UGameplayStatics::FinishSpawningActor(Doppelganger, SpawnTransform);
		Doppelganger->InitAbilityActorInfo();
		Doppelganger->GetKulkiMesh()->AddImpulse(Direction * ImpulseForce);
		
		UAbilitySystemComponent* SourceASC = GetActorInfo().AbilitySystemComponent.Get();
        const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DoppelgangerEffectClass, 1.f, SourceASC->MakeEffectContext());    
        SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Doppelganger->GetAbilitySystemComponent());
	}	
}
