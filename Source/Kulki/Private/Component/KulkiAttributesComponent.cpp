// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/KulkiAttributesComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UKulkiAttributesComponent::UKulkiAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UKulkiAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UKulkiAttributesComponent::SetStrengthAttribute(float NewStrength, UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed)
{
	Strength = UKismetMathLibrary::Clamp(NewStrength,0.f, 200);
	SetOwnerSize(Mesh, CapsuleCollision);
	SetOwnerSpeed(OutMovementSpeed);
	OnStrengthChangedDelegate.Broadcast(Strength);
	if (Strength <= 0)
	{
		// TODO: Game over
	}
}

void UKulkiAttributesComponent::AddToStrengthAttribute(float ValueToAdd, UStaticMeshComponent* Mesh,
                                                       UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed)
{
	const float NewStrength = Strength + ValueToAdd;
	SetStrengthAttribute(NewStrength, Mesh, CapsuleCollision, OutMovementSpeed);
}

void UKulkiAttributesComponent::SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed)
{
	Speed = UKismetMathLibrary::Clamp(NewSpeed, 0.f, 200);
	SetOwnerSpeed(OutMovementSpeed);
	OnSpeedChangedDelegate.Broadcast(Speed);
	if (Speed <= 0)
	{
		// TODO: Game over
	}
}

void UKulkiAttributesComponent::AddToSpeedAttribute(float ValueToAdd, float& OutMovementSpeed)
{
	const float NewSpeed = Speed + ValueToAdd;
	SetSpeedAttribute(NewSpeed, OutMovementSpeed);
}

void UKulkiAttributesComponent::SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision)
{
	if (CapsuleCollision && Mesh)
	{
		Mesh->SetWorldScale3D(FVector(Strength * 0.1f, Strength * 0.1f, 1.f));
		
		FVector Origin;
		FVector Bounds;
		float SphereRadius;
		UKismetSystemLibrary::GetComponentBounds(Mesh, Origin, Bounds, SphereRadius);
		const float FixedRadius = Bounds.X - CapsulePadding;
		CapsuleCollision->SetCapsuleHalfHeight(FixedRadius * 2.f);
		CapsuleCollision->SetCapsuleRadius(FixedRadius);		
	}
}

void UKulkiAttributesComponent::SetOwnerSpeed(float& OutMovementSpeed)
{
	const float NewValue = BaseMovementSpeed+ (Speed * 10.f) - (Strength * 10.f);
	OutMovementSpeed = UKismetMathLibrary::Clamp(NewValue, 200.f, 1500.f);
}


