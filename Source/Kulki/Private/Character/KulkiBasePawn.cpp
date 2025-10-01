// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#include "Character/KulkiBasePawn.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"
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

	AbilitySystemComponent = CreateDefaultSubobject<UKulkiAbilitySystemComponent>("AbilitySystemComponent");
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
	AddCharacterAbilities();
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
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data.Get());
}

void AKulkiBasePawn::AddCharacterAbilities()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddCharactersAbilities(StartupAbilities);
	}	
}

void AKulkiBasePawn::SetKulkiPawnSize(const FOnAttributeChangeData& Data)
{
	const float NewScale = FMath::Clamp((Data.NewValue * SizeMultiplier), 0.5f, 1000.f);
	SetActorScale3D(FVector(NewScale, NewScale, NewScale * 0.5));

	SetKulkiSizePenaltyMovementSpeed(Data.NewValue);
}

void AKulkiBasePawn::SetKulkiMovementSpeed(const FOnAttributeChangeData& Data)
{
	const float NewValue = BaseMovementSpeed + (Data.NewValue * SpeedMultiplier) - (AttributeSet->GetStrength() * SpeedPenaltyMultiplier);
	FloatingPawnMovement->MaxSpeed = FMath::Clamp(NewValue, MinMovementSpeed, MaxMovementSpeed);
}

void AKulkiBasePawn::SetKulkiSizePenaltyMovementSpeed(float Strength)
{
	const float NewValue = BaseMovementSpeed + (AttributeSet->GetSpeed() * SpeedMultiplier) - (Strength * SpeedPenaltyMultiplier);
	FloatingPawnMovement->MaxSpeed = FMath::Clamp(NewValue, MinMovementSpeed, MaxMovementSpeed);
}



