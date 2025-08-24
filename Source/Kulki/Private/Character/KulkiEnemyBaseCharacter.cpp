// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiEnemyBaseCharacter.h"
#include "Component/KulkiAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AKulkiEnemyBaseCharacter::AKulkiEnemyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Type = EEnemyType::NONE;

	KulkiMesh = CreateDefaultSubobject<UStaticMeshComponent>("KulkiMesh");
	KulkiMesh->SetupAttachment(RootComponent);
	KulkiMesh->CastShadow = false;

	AttackCapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCapsule"));
	AttackCapsuleCollision->SetupAttachment(RootComponent);

	DefendCapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DefendCapsule"));
	DefendCapsuleCollision->SetupAttachment(RootComponent);
	
	AttributesComponent = CreateDefaultSubobject<UKulkiAttributesComponent>("AttributesComponent");
}

void AKulkiEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributesComponent->SetStrengthAttribute(TempStrengthAttributeValue, KulkiMesh, AttackCapsuleCollision, GetCharacterMovement()->MaxWalkSpeed);
    AttributesComponent->SetSpeedAttribute(TempSpeedAttributeValue,GetCharacterMovement()->MaxWalkSpeed);
	
	SetMeshColor();
}

void AKulkiEnemyBaseCharacter::SetAttributesValue(float Strength, float Speed)
{
	TempStrengthAttributeValue = Strength;
	TempSpeedAttributeValue = Speed;
}

void AKulkiEnemyBaseCharacter::SetMeshColor()
{
	FLinearColor Color = FLinearColor::Gray;
	switch (Type)
	{
		case EEnemyType::YELLOW:
		{
			Color = FLinearColor::Yellow;
			break;
		}
		case EEnemyType::RED:
		{
			Color = FLinearColor::Red;
			break;
		}
		case EEnemyType::PURPLE:
		{
			Color = FLinearColor(0.5f, 0.f, 0.5f, 1.f);
			break;
		}
		default: break;
	}

	if (UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(KulkiMesh->GetMaterial(0), this))
	{
		DynMaterial->SetVectorParameterValue("MeshColor", Color);
		KulkiMesh->SetMaterial(0, DynMaterial);
	}
}



