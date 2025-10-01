// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


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
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor.Get());
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), OwnerASC);
	
	SetLifeSpan(0.01f);
}

void AKulkiDoppelgangerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Enable merging
	FTimerHandle MergeHandle;
	FTimerDelegate MergeDelegate;
	MergeDelegate.BindWeakLambda(this, [this]()
	{
		bCanMerge = true;
        OnCanMerge.Broadcast();
	});
	GetWorldTimerManager().SetTimer(MergeHandle, MergeDelegate, MergeTime, false);

	// Disable physics after being "launched" by owner
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
	
	const float EnemyStrength = CombatInterface->GetStrength();
	float Coefficient = -1.f;
	if (AttributeSet->GetStrength() >= EnemyStrength)
	{
		Coefficient = 1.f;
	}
	
	bool bEatableEnemy = false;
	CombatInterface->ApplyOverlapEffect(GetAbilitySystemComponent(), Coefficient, bEatableEnemy);
	if (Coefficient > 0.f)
	{
		const AKulkiPlayerPawn* Player = Cast<AKulkiPlayerPawn>(OwnerActor.Get());
		if (bEatableEnemy && Player)
		{
			Player->OnEatableEnemyKilled.ExecuteIfBound();			
		}
	}
	else
	{
		SetLifeSpan(0.01f);
	}
}
