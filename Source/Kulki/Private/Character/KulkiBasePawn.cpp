// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/KulkiBasePawn.h"
#include "AbilitySystem/KulkiAbilitySystemComponent.h"
#include "AbilitySystem/KulkiAttributeSet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AKulkiBasePawn::AKulkiBasePawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	KulkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KulkiMesh"));
	SetRootComponent(KulkiMesh);
	KulkiMesh->CastShadow = false;

	AttackSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphereCollision->SetupAttachment(RootComponent);
	AttackSphereCollision->SetSphereRadius(100.f);

	DefendSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DefendSphere"));
	DefendSphereCollision->SetupAttachment(RootComponent);
	DefendSphereCollision->SetSphereRadius(5.f);
	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(false);

	AttributeSet = CreateDefaultSubobject<UKulkiAttributeSet>(TEXT("AttributeSet"));
}

void AKulkiBasePawn::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AKulkiBasePawn::InitAbilityActorInfo()
{
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetStrengthAttribute()).AddUObject(this, &AKulkiBasePawn::SetKulkiPawnSize);
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetSpeedAttribute()).AddUObject(this, &AKulkiBasePawn::SetKulkiMovementSpeed);
	
	InitDefaultAttributes();
}

void AKulkiBasePawn::InitDefaultAttributes()
{
	if (DefaultAttributes)
	{
		ApplyEffectToSelf(DefaultAttributes, 1.f);
	}	
}

void AKulkiBasePawn::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) 
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AKulkiBasePawn::ApplyEffectToSelf | AbilitySystemComponent is null"));
	checkf(GameplayEffectClass, TEXT("AKulkiBasePawn::ApplyEffectToSelf | GameplayEffectClass is null"));

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void AKulkiBasePawn::AddCharacterAbilities()
{
	UKulkiAbilitySystemComponent* KulkiASC = CastChecked<UKulkiAbilitySystemComponent>(GetAbilitySystemComponent());
	KulkiASC->AddCharactersAbilities(StartupAbilities);
}

void AKulkiBasePawn::SetKulkiPawnSize(const FOnAttributeChangeData& Data)
{
	const float NewScale = FMath::Clamp((Data.NewValue * SizeMultiplier), 0.5f, 1000.f);
	SetActorScale3D(FVector(NewScale, NewScale, NewScale * 0.5));
}

void AKulkiBasePawn::SetKulkiMovementSpeed(const FOnAttributeChangeData& Data)
{
	const float NewValue = BaseMovementSpeed + (Data.NewValue * SpeedMultiplier) - (AttributeSet->GetStrength() * SpeedPenaltyMultiplier);
	FloatingPawnMovement->MaxSpeed = FMath::Clamp(NewValue, MinMovementSpeed, MaxMovementSpeed);
}



