// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/KulkiGameplayTags.h"
#include "GameplayTagsManager.h"

FKulkiGameplayTags FKulkiGameplayTags::GameplayTags;

void FKulkiGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attribute_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Strength"),
			FString("Increases size.")
		);

	GameplayTags.Attribute_MaxStrength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxStrength"),
			FString("Max value of Strength.")
		);

	GameplayTags.Attribute_Speed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Speed"),
		FString("Increases movement speed.")
		);

	GameplayTags.Attribute_MaxSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.MaxSpeed"),
		FString("Max value of Speed.")
		);
}
