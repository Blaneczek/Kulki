// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

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
	FGameplayTag InputTag;
};
