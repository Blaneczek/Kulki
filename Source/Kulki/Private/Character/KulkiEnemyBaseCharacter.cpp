// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiEnemyBaseCharacter.h"
#include "BlueprintEditor.h"
#include "Component/KulkiAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AKulkiEnemyBaseCharacter::AKulkiEnemyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Type = EEnemyType::NONE;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->CastShadow = false;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleCollision->SetupAttachment(RootComponent);
	
	AttributesComponent = CreateDefaultSubobject<UKulkiAttributesComponent>("AttributesComponent");
}

void AKulkiEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributesComponent->SetStrengthAttribute(DebugStrength, SphereMesh, CapsuleCollision, GetCharacterMovement()->MaxWalkSpeed);
    AttributesComponent->SetSpeedAttribute(DebugSpeed,GetCharacterMovement()->MaxWalkSpeed);

	SetMeshColor();
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

	if (UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(SphereMesh->GetMaterial(0), this))
	{
		DynMaterial->SetVectorParameterValue("MeshColor", Color);
		SphereMesh->SetMaterial(0, DynMaterial);
	}
}



