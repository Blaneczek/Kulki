// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Character/KulkiEnemyPawn.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/SphereComponent.h"
#include "Gameplay/KulkiGameplayTags.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"

AKulkiEnemyPawn::AKulkiEnemyPawn()
{
	StartAICheckSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StartAICheckSphere"));
	StartAICheckSphere->SetupAttachment(RootComponent);

	EndAICheckSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EndAICheckSphere"));
	EndAICheckSphere->SetupAttachment(RootComponent);

	StartAvoidanceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StartAvoidanceSphere"));
	StartAvoidanceSphere->SetupAttachment(RootComponent);

	EndAvoidanceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EndAvoidanceSphere"));
	EndAvoidanceSphere->SetupAttachment(RootComponent);
}

void AKulkiEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	SetMeshColor();

	StartAICheckSphere->OnComponentBeginOverlap.AddDynamic(this, &AKulkiEnemyPawn::OnStartAICheck);
	EndAICheckSphere->OnComponentEndOverlap.AddDynamic(this, &AKulkiEnemyPawn::OnEndAICheck);

	StartAvoidanceSphere->OnComponentBeginOverlap.AddDynamic(this, &AKulkiEnemyPawn::OnStartAvoidance);
	EndAvoidanceSphere->OnComponentEndOverlap.AddDynamic(this, &AKulkiEnemyPawn::OnEndAvoidance);
}

void AKulkiEnemyPawn::SetAttributesValue(float Strength, float Speed)
{
	checkf(SpawnAttributes, TEXT("AKulkiEnemyPawn::SpawnApplyEffectToSelf | SpawnAttributes is not valid"));

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(SpawnAttributes, 1.f, ContextHandle);
	
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::Attribute_Strength.GetTag(), Strength);
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::Attribute_Speed.GetTag(), Speed);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data.Get());
}

void AKulkiEnemyPawn::SetOverlapGameplayEffectClass(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	OverlapGameplayEffectClass = GameplayEffectClass;
}

void AKulkiEnemyPawn::InitSpawn(float Strength, float Speed)
{
	InitAbilityActorInfo();
	SetAttributesValue(Strength, Speed);
	SetAISpheresSize();
}

void AKulkiEnemyPawn::ApplyOverlapEffect(UAbilitySystemComponent* TargetASC, float Coefficient, bool& OutIsEatableEnemy)
{
	if (!TargetASC || !GetAbilitySystemComponent() || !OverlapGameplayEffectClass)
	{
		return;
	}

	OutIsEatableEnemy = Type == EEnemyType::RED || Type == EEnemyType::YELLOW;

	// If enemy is Purple and was smaller than player
	bool bWasSmallerPurple = !OutIsEatableEnemy && Coefficient > 0.f;
	
	// If enemy is Purple decrease player's stats regardless size
	Coefficient = OutIsEatableEnemy ? Coefficient : -1.f;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(OverlapGameplayEffectClass, 1.f, ContextHandle);
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::GameplayEffect_Coefficient.GetTag(), Coefficient);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), TargetASC);

	// Enemy was smaller than player
	if (bWasSmallerPurple || Coefficient > 0.f)
	{		
		if (AAIController* AIC = Cast<AAIController>(GetController()))
		{
			AIC->GetBrainComponent()->StopLogic("");		
		}
		SetLifeSpan(0.01f);
	}
}

float AKulkiEnemyPawn::GetStrength() const
{
	return AttributeSet ? AttributeSet->GetStrength() : 0.f;
}

void AKulkiEnemyPawn::ResetNeighbors()
{
	AvoidanceNeighbors.Empty();
	
	TArray<AActor*> Neighbours;
	StartAvoidanceSphere->GetOverlappingActors(Neighbours, StaticClass());
	for (AActor* Actor : Neighbours)
	{
		if (Actor != this)
		{
			AvoidanceNeighbors.Add(Actor);
		}		
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

void AKulkiEnemyPawn::SetAISpheresSize()
{
	const float Scale = GetActorScale().X;
	StartAICheckSphere->SetSphereRadius((Scale * 10.f) + StartAICheckRadius);
	EndAICheckSphere->SetSphereRadius((Scale * 10.f) + EndAICheckRadius);
}

void AKulkiEnemyPawn::OnStartAICheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIdleState && OtherActor)
	{
		OnCheckIfBigger.ExecuteIfBound();
		bIdleState = false;
	}
}

void AKulkiEnemyPawn::OnEndAICheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIdleState && OtherActor)
	{
		bIdleState = true;
		OnBackToIdle.ExecuteIfBound();
	}
}

void AKulkiEnemyPawn::OnStartAvoidance(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIdleState && OtherActor)
	{
		AvoidanceNeighbors.Add(OtherActor);
	}
}

void AKulkiEnemyPawn::OnEndAvoidance(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIdleState && OtherActor)
	{
		AvoidanceNeighbors.Remove(OtherActor);
	}
}

