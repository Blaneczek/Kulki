// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "KulkiAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStrengthReachedZero);

/**
 * 
 */
UCLASS()
class KULKI_API UKulkiAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UKulkiAttributeSet();

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UKulkiAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS_BASIC(UKulkiAttributeSet, MaxStrength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS_BASIC(UKulkiAttributeSet, Speed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UKulkiAttributeSet, MaxSpeed);

	FOnStrengthReachedZero OnAttributeReachedZero;
};
