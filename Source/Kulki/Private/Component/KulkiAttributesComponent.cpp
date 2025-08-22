// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/KulkiAttributesComponent.h"

#include "Components/CapsuleComponent.h"

UKulkiAttributesComponent::UKulkiAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UKulkiAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UKulkiAttributesComponent::SetStrengthValue(float NewStrength)
{
	Strength = NewStrength;
	OnStrengthChangedDelegate.Broadcast(Strength);
}

void UKulkiAttributesComponent::SetSpeedValue(float NewSpeed)
{
	Speed = NewSpeed;
	OnSpeedChangedDelegate.Broadcast(Speed);
}

void UKulkiAttributesComponent::SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision)
{
	if (Mesh && CapsuleCollision)
	{
		Mesh->SetWorldScale3D(FVector(Strength, Strength, 1.f));
		CapsuleCollision->SetCapsuleRadius((Strength * 20.f)); //TODO: hardcoded
	}	
}

void UKulkiAttributesComponent::SetOwnerSpeed()
{
}


