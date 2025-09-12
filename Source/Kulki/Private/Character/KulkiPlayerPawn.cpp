// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerPawn.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/KulkiAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Character/KulkiEnemyPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/KulkiGameMode.h"
#include "Gameplay/KulkiCombatInterface.h"
#include "Gameplay/KulkiGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Player/KulkiPlayerController.h"
#include "UI/KulkiHUD.h"

AKulkiPlayerPawn::AKulkiPlayerPawn()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetRelativeRotation(FRotator(-90.f, 0.0f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
}

void AKulkiPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	AttackSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerPawn::OnOverlapAttack);
	DefendSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerPawn::OnOverlapAttack);

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(KulkiMesh->GetMaterial(0), this);
}

void AKulkiPlayerPawn::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AttributeSet->OnAttributeReachedZero.AddUObject(this, &AKulkiPlayerPawn::OnPlayerLost);
	
	if (AKulkiPlayerController* KulkiPC = Cast<AKulkiPlayerController>(GetController()))
	{
		if (AKulkiHUD* KulkiHUD = Cast<AKulkiHUD>(KulkiPC->GetHUD()))
		{
			KulkiHUD->InitOverlayWidget(KulkiPC, this, GetAbilitySystemComponent(), GetAttributeSet());
		}
	}	
}

void AKulkiPlayerPawn::OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsImmune)
	{
		return;
	}
	
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
			if (bEatableEnemy) OnEatableEnemyKilled.ExecuteIfBound();
		}
		else
		{
			ActivateImmunity();
		}
	}
}

void AKulkiPlayerPawn::OnPlayerLost()
{
	bIsImmune = true;
	FloatingPawnMovement->Deactivate();
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (AKulkiGameMode* GameMode = Cast<AKulkiGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->ResetGame();
	}
}

void AKulkiPlayerPawn::ActivateImmunity()
{
	OnImmunityActivation.ExecuteIfBound();	
	bIsImmune = true;

	FLinearColor BaseColor = FLinearColor::Green;
	if (IsValid(DynamicMaterialInstance))
	{
		FMaterialParameterInfo ParamInfo = FMaterialParameterInfo(TEXT("MeshColor"));
		DynamicMaterialInstance->GetVectorParameterValue(ParamInfo, BaseColor);
		DynamicMaterialInstance->SetVectorParameterValue("MeshColor", ImmunityColor);
		KulkiMesh->SetMaterial(0, DynamicMaterialInstance);
	}
	
	FTimerHandle ImmunityTimer;
	FTimerDelegate ImmunityDelegate;
	ImmunityDelegate.BindUObject(this, &AKulkiPlayerPawn::DeactivateImmunity, BaseColor);
	GetWorldTimerManager().SetTimer(ImmunityTimer, ImmunityDelegate, ImmunityTime, false);
}

void AKulkiPlayerPawn::DeactivateImmunity(const FLinearColor Color)
{
	OnImmunityDeactivation.ExecuteIfBound();
	bIsImmune = false;
	
	if (IsValid(DynamicMaterialInstance))
	{
		DynamicMaterialInstance->SetVectorParameterValue("MeshColor", Color);
		KulkiMesh->SetMaterial(0, DynamicMaterialInstance);
	}		
}

void AKulkiPlayerPawn::EnemyHitApplyEffectToSelf(APawn* Enemy, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, float Coefficient)
{
	if (!GameplayEffectClass)
	{
		return;
	}
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	ContextHandle.AddInstigator(Enemy, Enemy);
	const FGameplayEffectSpecHandle GameplayEffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GameplayEffectSpec.Data->SetSetByCallerMagnitude(KulkiGameplayTags::GameplayEffect_Coefficient.GetTag(), Coefficient);
	
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpec.Data.Get(), GetAbilitySystemComponent());
}
