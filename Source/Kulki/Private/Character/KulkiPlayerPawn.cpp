// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerPawn.h"
#include "AbilitySystem/KulkiAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Character/KulkiEnemyPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/KulkiGameMode.h"
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
	/*if (bIsImmune)
	{
		return;
	}

	AKulkiEnemyPawn* Enemy = Cast<AKulkiEnemyPawn>(OtherActor);
	if (!Enemy)
	{
		return;
	}

	// If Enemy is bigger than Player, we want negative number to subtract Attribute value 
	float Helper = -1.f;
	if (GetAttributesComponent()->StrengthAttribute.Value >= Enemy->GetAttributesComponent()->StrengthAttribute.Value)
	{
		Helper = 1.f;
	}

	// Added/subtracted value depends on Enemy size (Strength)
	const float EnemyStrength = Enemy->GetAttributesComponent()->StrengthAttribute.Value * Helper;
	
	bool bEatable = false;
	
	switch (Enemy->Type)
	{
		case EEnemyType::RED:
		{
			bEatable = true;
			AttributesComponent->AddToStrengthAttribute(EnemyStrength , FloatingPawnMovement->MaxSpeed);
			break;
		}
		case EEnemyType::YELLOW:
		{
			bEatable = true;
			AttributesComponent->AddToSpeedAttribute(EnemyStrength, FloatingPawnMovement->MaxSpeed);
			break;
		}
		case EEnemyType::PURPLE:
		{
			// Purple Enemy always subtracts Player's attributes
			bEatable = false;
			AttributesComponent->AddToStrengthAttribute(FMath::Abs(EnemyStrength) * (-1.f), FloatingPawnMovement->MaxSpeed);
			AttributesComponent->AddToSpeedAttribute(FMath::Abs(EnemyStrength) * (-1.f), FloatingPawnMovement->MaxSpeed);
			break;
		}
		default: break;
	}

	// Destroy Enemy if it is smaller 
	if (Helper > 0.f)
	{
		Enemy->Destroy();
		if (bEatable) OnEnemyKilled.ExecuteIfBound();		
	}
	else
	{
		// Give Player immunity after being caught 
		ActivateImmunity();
	}*/
}

void AKulkiPlayerPawn::OnPlayerLost()
{
	bIsImmune = true;
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