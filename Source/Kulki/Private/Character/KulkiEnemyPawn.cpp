// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiEnemyPawn.h"
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
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::Attribute_Strength.GetTag(), Strength);
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::Attribute_Speed.GetTag(), Speed);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void AKulkiEnemyPawn::SetSpawnOverlapGameplayEffectClass(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	OverlapGameplayEffectClass = GameplayEffectClass;
}

void AKulkiEnemyPawn::ApplyOverlapEffect(UAbilitySystemComponent* TargetASC, float Coefficient, bool& OutIsEatableEnemy)
{
	if (!TargetASC || !GetAbilitySystemComponent() || !OverlapGameplayEffectClass)
	{
		return;
	}

	OutIsEatableEnemy = Type == EEnemyType::RED || Type == EEnemyType::YELLOW;
	Coefficient = OutIsEatableEnemy ? Coefficient : -1.f;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(OverlapGameplayEffectClass, 1.f, ContextHandle);
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::GameplayEffect_Coefficient.GetTag(), Coefficient);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), TargetASC);

	if (Coefficient > 0.f)
	{
		Destroy();
	}
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

