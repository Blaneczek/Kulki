// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Component/KulkiAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/KulkiGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/KulkiHUD.h"

AKulkiPlayerCharacter::AKulkiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	KulkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KulkiMesh"));
	KulkiMesh->SetupAttachment(RootComponent);
	KulkiMesh->CastShadow = false;

	AttackCapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCapsule"));
	AttackCapsuleCollision->SetupAttachment(RootComponent);

	DefendCapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DefendCapsule"));
	DefendCapsuleCollision->SetupAttachment(RootComponent);
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetRelativeRotation(FRotator(-90.f, 0.0f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;

	AttributesComponent = CreateDefaultSubobject<UKulkiAttributesComponent>("AttributesComponent");
}

void AKulkiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set default attributes
	AttributesComponent->SetStrengthAttribute(BaseStrengthAttributeValue, KulkiMesh, AttackCapsuleCollision, MovementSpeed);
	AttributesComponent->SetSpeedAttribute(BaseSpeedAttributeValue, MovementSpeed);

	AttributesComponent->OnAttributeReachedZero.BindLambda([this]()
		{
			bIsImmune = true;
			MovementForce = 0.f;
			GetWorldTimerManager().ClearAllTimersForObject(this);
			if (AKulkiGameMode* GameMode = Cast<AKulkiGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				GameMode->ResetGame();
			}
		});
	
	// Init Main widget with controller 
	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (AKulkiHUD* KulkiHUD = Cast<AKulkiHUD>(PC->GetHUD()))
		{
			KulkiHUD->InitOverlayWidget(this);
		}
	}

	AttackCapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerCharacter::OnOverlapAttack);
	DefendCapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerCharacter::OnOverlapAttack);

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(KulkiMesh->GetMaterial(0), this);
}

void AKulkiPlayerCharacter::OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsImmune)
	{
		return;
	}
	
	AKulkiEnemyBaseCharacter* Enemy = Cast<AKulkiEnemyBaseCharacter>(OtherActor);
	if (!Enemy)
	{
		return;
	}

	// If Enemy is bigger than Player, we want negative number 
	float Helper = -1.f;
	if (AttributesComponent->StrengthAttribute.Value >= Enemy->GetAttributesComponent()->StrengthAttribute.Value)
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
			AttributesComponent->AddToStrengthAttribute(EnemyStrength , KulkiMesh, AttackCapsuleCollision, MovementSpeed);
			break;
		}
		case EEnemyType::YELLOW:
		{
			bEatable = true;
			AttributesComponent->AddToSpeedAttribute(EnemyStrength, MovementSpeed);
			break;
		}
		case EEnemyType::PURPLE:
		{
			// Purple Enemy always subtracts player's attributes
			bEatable = false;
			AttributesComponent->AddToStrengthAttribute(FMath::Abs(EnemyStrength) * (-1.f), KulkiMesh, AttackCapsuleCollision, MovementSpeed);
			AttributesComponent->AddToSpeedAttribute(FMath::Abs(EnemyStrength) * (-1.f), MovementSpeed);
			break;
		}
		default: break;
	}

	// Destroys Enemy if it is smaller 
	if (Helper > 0.f)
	{
		Enemy->Destroy();
		if (bEatable) OnEnemyKilled.ExecuteIfBound();		
	}
	else
	{
		// Gives Player immunity after being caught 
		ActivateImmunity();
	}
}

void AKulkiPlayerCharacter::OnOverlapDefend(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AKulkiPlayerCharacter::ActivateImmunity()
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
	ImmunityDelegate.BindUObject(this, &AKulkiPlayerCharacter::DeactivateImmunity, BaseColor);
	GetWorldTimerManager().SetTimer(ImmunityTimer, ImmunityDelegate, ImmunityTime, false);
}

void AKulkiPlayerCharacter::DeactivateImmunity(FLinearColor Color)
{
	OnImmunityDeactivation.ExecuteIfBound();
	bIsImmune = false;
	
	if (IsValid(DynamicMaterialInstance))
	{
		DynamicMaterialInstance->SetVectorParameterValue("MeshColor", Color);
		KulkiMesh->SetMaterial(0, DynamicMaterialInstance);
	}		
}




