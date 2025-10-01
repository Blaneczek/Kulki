// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KulkiCombatInterface.generated.h"

class UAbilitySystemComponent;

UINTERFACE(MinimalAPI)
class UKulkiCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KULKI_API IKulkiCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyOverlapEffect(UAbilitySystemComponent* TargetASC, float Coefficient, bool& OutIsEatableEnemy) {};
	virtual float GetStrength() const { return 0.f; }
};
