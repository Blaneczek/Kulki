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
	AKulkiEnemyBaseCharacter* Enemy = Cast<AKulkiEnemyBaseCharacter>(OtherActor);
	if (!Enemy)
	{
		return;
	}
	
	float Helper = -1.f;
	if (AttributesComponent->StrengthAttribute.Value >= Enemy->GetAttributesComponent()->StrengthAttribute.Value)
	{
		Helper = 1.f;
	}

	switch (Enemy->Type)
	{
		case EEnemyType::RED:
		{
			const float EnemyStrength = Enemy->GetAttributesComponent()->StrengthAttribute.Value * Helper;	
			AttributesComponent->AddToStrengthAttribute(EnemyStrength , SphereMesh, CapsuleCollision, MovementSpeed);
			break;
		}
		case EEnemyType::YELLOW:
		{
			const float EnemySpeed = Enemy->GetAttributesComponent()->SpeedAttribute.Value * Helper;	
			AttributesComponent->AddToSpeedAttribute(EnemySpeed, MovementSpeed);
			break;
		}
		case EEnemyType::PURPLE:
		{
			const float EnemyStrength = Enemy->GetAttributesComponent()->StrengthAttribute.Value * Helper;
			const float EnemySpeed = Enemy->GetAttributesComponent()->SpeedAttribute.Value * Helper;
			AttributesComponent->AddToStrengthAttribute(-UKismetMathLibrary::Abs(EnemyStrength), SphereMesh, CapsuleCollision, MovementSpeed);
			AttributesComponent->AddToSpeedAttribute(-UKismetMathLibrary::Abs(EnemySpeed), MovementSpeed);
			break;
		}
		default: break;
	}

	if (Helper > 0.f)
	{
		Enemy->Destroy();
	}
}




