// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/**
 * KulkiGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FKulkiGameplayTags
{
public:
	static const FKulkiGameplayTags& Get() { return GameplayTags; }
	
	static void InitializeNativeGameplayTags();

	FGameplayTag Attribute_Strength;
	FGameplayTag Attribute_MaxStrength;
	
	FGameplayTag Attribute_Speed;
	FGameplayTag Attribute_MaxSpeed;
	
private:
	static FKulkiGameplayTags GameplayTags;
};

