// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Character/KulkiPlayerPawn.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/AbilitySystem/KulkiAttributeSet.h"
#include "Character/KulkiEnemyPawn.h"
#include "Component/KulkiCameraComponent.h"
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
	
	Camera = CreateDefaultSubobject<UKulkiCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
}

void AKulkiPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddAbilities();
}

void AKulkiPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	AttackSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerPawn::OnOverlap);
	DefendSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerPawn::OnOverlap);

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(KulkiMesh->GetMaterial(0), this);
}

void AKulkiPlayerPawn::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	AttributeSet->OnAttributeReachedZero.AddUObject(this, &AKulkiPlayerPawn::OnPlayerLost);
	
	InitDefaultAttributes();
		
	// Create UI
	if (AKulkiPlayerController* KulkiPC = Cast<AKulkiPlayerController>(GetController()))
	{
		if (AKulkiHUD* KulkiHUD = Cast<AKulkiHUD>(KulkiPC->GetHUD()))
		{
			KulkiHUD->InitOverlayWidget(KulkiPC, this, GetAbilitySystemComponent(), GetAttributeSet());
		}
	}	
}

void AKulkiPlayerPawn::AddAbilities()
{
	AbilitySystemComponent->AddCharactersAbilities(StartupAbilities);
}

void AKulkiPlayerPawn::InitDefaultAttributes()
{
	ApplyEffectToSelf(DefaultAttributes, 1.f);
}

void AKulkiPlayerPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	IKulkiCombatInterface* CombatInterface = Cast<IKulkiCombatInterface>(OtherActor);
	
	if (bIsImmune || !CombatInterface)
	{
		return;
	}
	
	const float EnemyStrength = CombatInterface->GetStrength();
	float Coefficient = -1.f;

	// If Player is bigger, the coefficient is positive to increase Player's attributes
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
	bIsImmune = true;
	FLinearColor BaseColor = FLinearColor::Green;
	
	if (DynamicMaterialInstance)
	{
		const FMaterialParameterInfo ParamInfo = FMaterialParameterInfo(TEXT("MeshColor"));
		DynamicMaterialInstance->GetVectorParameterValue(ParamInfo, BaseColor);
		DynamicMaterialInstance->SetVectorParameterValue("MeshColor", ImmunityColor);
		KulkiMesh->SetMaterial(0, DynamicMaterialInstance);
	}
	
	FTimerHandle ImmunityTimer;
	FTimerDelegate ImmunityDelegate;
	ImmunityDelegate.BindUObject(this, &AKulkiPlayerPawn::DeactivateImmunity, BaseColor);
	GetWorldTimerManager().SetTimer(ImmunityTimer, ImmunityDelegate, ImmunityTime, false);
	
	OnImmunityActivation.Broadcast();
}

void AKulkiPlayerPawn::DeactivateImmunity(const FLinearColor Color)
{	
	bIsImmune = false;
	
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue("MeshColor", Color);
		KulkiMesh->SetMaterial(0, DynamicMaterialInstance);
	}
	
	OnImmunityDeactivation.Broadcast();	
}

void AKulkiPlayerPawn::SetKulkiPawnSize(const FOnAttributeChangeData& Data)
{
	Super::SetKulkiPawnSize(Data);
	
	OnStrengthChanged.Broadcast();
	Camera->ChangeOrtoWidth(Data.NewValue);
}

