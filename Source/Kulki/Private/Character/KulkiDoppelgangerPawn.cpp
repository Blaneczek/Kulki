// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiDoppelgangerPawn.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/KulkiPlayerPawn.h"
#include "Components/SphereComponent.h"
#include "Gameplay/KulkiCombatInterface.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"

AKulkiDoppelgangerPawn::AKulkiDoppelgangerPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	MergeTime = 0.f;
	KulkiMesh->SetSimulatePhysics(true);
}

void AKulkiDoppelgangerPawn::SetData(AActor* AbilityOwner, float InMergeTime)
{
	OwnerActor = AbilityOwner;
	MergeTime = InMergeTime;
}

void AKulkiDoppelgangerPawn::MergeWithOwner()
{
	if (!MergeEffectClass || !GetAbilitySystemComponent() || !OwnerActor.IsValid())
	{
		SetLifeSpan(0.01f);
		return;
	}
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(MergeEffectClass, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor.Get()));
	
	SetLifeSpan(0.01f);
}


void AKulkiDoppelgangerPawn::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle MergeHandle;
	FTimerDelegate MergeDelegate;
	MergeDelegate.BindWeakLambda(this, [this]()
	{
		bCanMerge = true;
        OnCanMerge.Broadcast();
	});
	GetWorldTimerManager().SetTimer(MergeHandle, MergeDelegate, MergeTime, false);

	FTimerHandle ImpulseHandle;
	FTimerDelegate ImpulseDelegate;
	ImpulseDelegate.BindWeakLambda(this, [this]()
	{
		GetKulkiMesh()->SetSimulatePhysics(false);
	});
	GetWorldTimerManager().SetTimer(ImpulseHandle, ImpulseDelegate, 2.f, false);

	AttackSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiDoppelgangerPawn::OnOverlap);
	DefendSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiDoppelgangerPawn::OnOverlap);
}

void AKulkiDoppelgangerPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IKulkiCombatInterface* CombatInterface = Cast<IKulkiCombatInterface>(OtherActor);
	if (!CombatInterface)
	{
		return;
	}
	
	bool bFoundStrengthAttribute = false;
	const float EnemyStrength = UAbilitySystemBlueprintLibrary::GetFloatAttribute(OtherActor, UKulkiAttributeSet::GetStrengthAttribute(), bFoundStrengthAttribute);
	if (bFoundStrengthAttribute)
	{
		float Coefficient = -1.f;
		if (AttributeSet->GetStrength() >= EnemyStrength)
		{
			Coefficient = 1.f;
		}
		bool bEatableEnemy = false;
		CombatInterface->ApplyOverlapEffect(GetAbilitySystemComponent(), Coefficient, bEatableEnemy);
		if (Coefficient > 0.f)
		{
			if (const AKulkiPlayerPawn* Player = Cast<AKulkiPlayerPawn>(OwnerActor.Get()); bEatableEnemy && Player)
			{
				Player->OnEatableEnemyKilled.ExecuteIfBound();			
			}
		}
		else
		{
			SetLifeSpan(0.01f);
		}
	}
}
