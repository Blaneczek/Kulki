// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KulkiGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KULKI_API UKulkiGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UKulkiGameplayAbility();
	
	UPROPERTY(EditDefaultsOnly, Category = "Kulki|Input")
	FGameplayTag StartupInputTag;
};
