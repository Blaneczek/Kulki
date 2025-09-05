// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiEnemyPawn.h"
#include "Component/KulkiAttributesComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AKulkiEnemyPawn::AKulkiEnemyPawn()
{
	Type = EEnemyType::NONE;
}

void AKulkiEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	GetAttributesComponent()->SetStrengthAttribute(TempStrengthAttributeValue, KulkiMesh, AttackSphereCollision, FloatingPawnMovement->MaxSpeed);
	GetAttributesComponent()->SetSpeedAttribute(TempSpeedAttributeValue, FloatingPawnMovement->MaxSpeed);
	
	SetMeshColor();
}

void AKulkiEnemyPawn::SetAttributesValue(float Strength, float Speed)
{
	TempStrengthAttributeValue = Strength;
	TempSpeedAttributeValue = Speed;
}

void AKulkiEnemyPawn::SetMeshColor()
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