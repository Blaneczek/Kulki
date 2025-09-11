// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_Attribute.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/KulkiAttributeSet.h"
#include "Gameplay/KulkiGameplayTags.h"

UMMC_Attribute::UMMC_Attribute()
{
	StrengthDef.AttributeToCapture = UKulkiAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_Attribute::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const float Coefficient = Spec.GetSetByCallerMagnitude(KulkiGameplayTags::GameplayEffect_Coefficient.GetTag(), false, 1.f);

	FAggregatorEvaluateParameters Params;
	float AttributeValue = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, Params, AttributeValue);

	const float CurveValue = AttributeCurve ? AttributeCurve->GetFloatValue(AttributeValue) : 0.f;
	
	return Coefficient * CurveValue;
}
