// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/KulkiGameplayTags.h"

namespace KulkiGameplayTags
{
 	KULKI_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Strength, "Attribute.Strength", "Increases size.");
 	KULKI_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MaxStrength, "Attribute.MaxStrength", "Max value of Strength.");
	KULKI_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Speed, "Attribute.Speed", "Increases movement speed.");
	KULKI_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MaxSpeed, "Attribute.MaxSpeed", "Max value of Speed.");
	
	KULKI_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_Coefficient, "GameplayEffect.Coefficient", "SetByCaller multiplier used to flip or scale the effect (e.g., +1 for buff, -1 for debuff.");
}