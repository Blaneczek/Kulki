// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KulkiAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KULKI_API UKulkiAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UKulkiAbilitySystemComponent();

	void AddCharactersAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
protected:
	virtual void BeginPlay() override;


};
