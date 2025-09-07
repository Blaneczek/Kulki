// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiEnemyPawn.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/KulkiGameplayTags.h"

AKulkiEnemyPawn::AKulkiEnemyPawn()
{
}

void AKulkiEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	SetMeshColor();

}

void AKulkiEnemyPawn::SetSpawnAttributesValue(float Strength, float Speed)
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AKulkiEnemyPawn::SpawnApplyEffectToSelf | AbilitySystemComponent is null"));
	checkf(SpawnAttributes, TEXT("AKulkiEnemyPawn::SpawnApplyEffectToSelf | SpawnAttributes is null"));

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(SpawnAttributes, 1.f, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpec, KulkiGameplayTags::Attribute_Strength.GetTag(), Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpec, KulkiGameplayTags::Attribute_Speed.GetTag(), Speed);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void AKulkiEnemyPawn::SetMeshColor()
{
	FLinearColor Color = FLinearColor::Gray;
	switch (Type)
	{
		case EEnemyType::YELLOW:
		{
			Color = FLinearColor::Yellow;
			break;
		}
		case EEnemyType::RED:
		{
			Color = FLinearColor::Red;
			break;
		}
		case EEnemyType::PURPLE:
		{
			Color = FLinearColor(0.5f, 0.f, 0.5f, 1.f);
			break;
		}
		default: break;
	}

	if (UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(KulkiMesh->GetMaterial(0), this))
	{
		DynMaterial->SetVectorParameterValue("MeshColor", Color);
		KulkiMesh->SetMaterial(0, DynMaterial);
	}
}

