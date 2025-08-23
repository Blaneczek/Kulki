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
	StrengthAttribute.Value = UKismetMathLibrary::Clamp(NewStrength,0.f, StrengthAttribute.MaxValue);
	SetOwnerSize(Mesh, CapsuleCollision);
	SetOwnerSpeed(OutMovementSpeed);
	OnStrengthChangedDelegate.Broadcast(StrengthAttribute.Value);
	if (StrengthAttribute.Value <= 0)
	{
		// TODO: Game over
	}
}

void UKulkiAttributesComponent::AddToStrengthAttribute(float EnemyStrength, UStaticMeshComponent* Mesh,
                                                       UCapsuleComponent* CapsuleCollision, float& OutMovementSpeed)
{
	if (StrengthAttribute.AddToValueCurve)
	{
		const float NewStrength = StrengthAttribute.Value + StrengthAttribute.AddToValueCurve->GetFloatValue(EnemyStrength);
		SetStrengthAttribute(NewStrength, Mesh, CapsuleCollision, OutMovementSpeed);
	}
}

void UKulkiAttributesComponent::SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed)
{
	SpeedAttribute.Value = UKismetMathLibrary::Clamp(NewSpeed, 0.f, SpeedAttribute.MaxValue);
	SetOwnerSpeed(OutMovementSpeed);
	OnSpeedChangedDelegate.Broadcast(SpeedAttribute.Value );
	if (SpeedAttribute.Value  <= 0)
	{
		// TODO: Game over
	}
}

void UKulkiAttributesComponent::AddToSpeedAttribute(float EnemyStrength, float& OutMovementSpeed)
{
	if (SpeedAttribute.AddToValueCurve)
	{
		const float NewSpeed = SpeedAttribute.Value + SpeedAttribute.AddToValueCurve->GetFloatValue(EnemyStrength);
		SetSpeedAttribute(NewSpeed, OutMovementSpeed);
	}
}

void UKulkiAttributesComponent::SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* CapsuleCollision)
{
	if (CapsuleCollision && Mesh)
	{
		Mesh->SetWorldScale3D(FVector(StrengthAttribute.Value * 0.1f, StrengthAttribute.Value * 0.1f, 1.f));
		
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
	const float NewValue = BaseMovementSpeed + (SpeedAttribute.Value * SpeedMultiplier) - (StrengthAttribute.Value * SpeedPenaltyMultiplier);
	OutMovementSpeed = UKismetMathLibrary::Clamp(NewValue, MinMovementSpeed, MaxMovementSpeed);
}


