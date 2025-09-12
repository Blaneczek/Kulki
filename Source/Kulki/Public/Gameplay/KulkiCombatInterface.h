// Fill out your copyright notice in the Description page of Project Settings.

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
};
