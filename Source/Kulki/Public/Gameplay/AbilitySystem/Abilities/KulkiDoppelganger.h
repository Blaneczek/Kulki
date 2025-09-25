// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KulkiGameplayAbility.h"
#include "KulkiDoppelganger.generated.h"

class AKulkiDoppelgangerPawn;
/**
 * 
 */
UCLASS()
class KULKI_API UKulkiDoppelganger : public UKulkiGameplayAbility
{
	GENERATED_BODY()
public:
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Kulki")
	void SpawnDoppelganger(const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TSubclassOf<AKulkiDoppelgangerPawn> DoppelgangerPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TSubclassOf<UGameplayEffect> DoppelgangerEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	float MergeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	float ImpulseForce = 1000.f;
};
