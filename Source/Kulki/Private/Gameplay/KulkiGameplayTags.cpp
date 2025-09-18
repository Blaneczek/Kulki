// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/KulkiGameplayTags.h"

namespace KulkiGameplayTags
{
 	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Strength, "Attribute.Strength", "Increases size.");
 	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MaxStrength, "Attribute.MaxStrength", "Max value of Strength.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Speed, "Attribute.Speed", "Increases movement speed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MaxSpeed, "Attribute.MaxSpeed", "Max value of Speed.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_Coefficient, "GameplayEffect.Coefficient", "SetByCaller multiplier used to flip or scale the effect (e.g., +1 for buff, -1 for debuff.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Spacebar, "Input.Spacebar", "Spacebar input.");
	
}