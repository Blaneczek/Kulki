// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/KulkiEnemyBaseCharacter.h"
#include "Component/KulkiAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/KulkiHUD.h"

AKulkiPlayerCharacter::AKulkiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->CastShadow = false;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleCollision->SetupAttachment(RootComponent);
	
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
	AttributesComponent->SetStrengthAttribute(DefaultStrengthAttribute, SphereMesh, CapsuleCollision, MovementSpeed);
	AttributesComponent->SetSpeedAttribute(DefaultSpeedAttribute, MovementSpeed);
	
	// Init Main widget with controller 
	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (AKulkiHUD* KulkiHUD = Cast<AKulkiHUD>(PC->GetHUD()))
		{
			KulkiHUD->InitOverlayWidget(this);
		}
	}

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AKulkiPlayerCharacter::OnOverlap);
}

void AKulkiPlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
	
	switch (Enemy->Type)
	{
		case EEnemyType::RED:
		{		
			AttributesComponent->AddToStrengthAttribute(EnemyStrength , SphereMesh, CapsuleCollision, MovementSpeed);
			break;
		}
		case EEnemyType::YELLOW:
		{
			AttributesComponent->AddToSpeedAttribute(EnemyStrength, MovementSpeed);
			break;
		}
		case EEnemyType::PURPLE:
		{
			// Purple Enemy always subtracts player's attributes
			AttributesComponent->AddToStrengthAttribute(-UKismetMathLibrary::Abs(EnemyStrength), SphereMesh, CapsuleCollision, MovementSpeed);
			AttributesComponent->AddToSpeedAttribute(-UKismetMathLibrary::Abs(EnemyStrength), MovementSpeed);
			break;
		}
		default: break;
	}

	// Destroys Enemy if it is smaller 
	if (Helper > 0.f)
	{
		Enemy->Destroy();
	}
	else
	{
		// Gives Player immunity after being caught 
		ActivateImmunity();
		//TODO: Give player immunity and set Enemy AI to Idle State
	}
}

void AKulkiPlayerCharacter::ActivateImmunity()
{
	bIsImmune = true;
}

void AKulkiPlayerCharacter::DeactivateImmunity()
{
	bIsImmune = false;
}




