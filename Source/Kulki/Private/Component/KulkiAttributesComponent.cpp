// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/KulkiAttributesComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UKulkiAttributesComponent::UKulkiAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UKulkiAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
			
}

void UKulkiAttributesComponent::SetStrengthAttribute(float NewStrength, UStaticMeshComponent* Mesh,
													UCapsuleComponent* AttackCapsuleCollision, float& OutMovementSpeed)
{
	StrengthAttribute.Value = FMath::Clamp(NewStrength,0.f, StrengthAttribute.MaxValue);
	SetOwnerSize(Mesh, AttackCapsuleCollision);
	SetOwnerSpeed(OutMovementSpeed);
	OnStrengthChangedDelegate.Broadcast(StrengthAttribute.Value);
	if (StrengthAttribute.Value <= 0)
	{
		OnAttributeReachedZero.ExecuteIfBound();
	}
}

void UKulkiAttributesComponent::AddToStrengthAttribute(float EnemyStrength, UStaticMeshComponent* Mesh,
													UCapsuleComponent* AttackCapsuleCollision, float& OutMovementSpeed)
{
	if (StrengthAttribute.AddToValueCurve)
	{
		const float NewStrength = StrengthAttribute.Value + StrengthAttribute.AddToValueCurve->GetFloatValue(EnemyStrength);
		SetStrengthAttribute(NewStrength, Mesh, AttackCapsuleCollision, OutMovementSpeed);
	}
}

void UKulkiAttributesComponent::SetSpeedAttribute(float NewSpeed, float& OutMovementSpeed)
{
	SpeedAttribute.Value = FMath::Clamp(NewSpeed, 0.f, SpeedAttribute.MaxValue);
	SetOwnerSpeed(OutMovementSpeed);
	OnSpeedChangedDelegate.Broadcast(SpeedAttribute.Value );
	if (SpeedAttribute.Value  <= 0)
	{
		OnAttributeReachedZero.ExecuteIfBound();
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

void UKulkiAttributesComponent::SetOwnerSize(UStaticMeshComponent* Mesh, UCapsuleComponent* AttackCapsuleCollision)
{
	if (Mesh && AttackCapsuleCollision)
	{
		const float NewScale = FMath::Clamp((StrengthAttribute.Value * SizeMultiplier), 0.5f, 1000.f);
		Mesh->SetWorldScale3D(FVector(NewScale, NewScale, NewScale));

		FVector Origin;
		FVector Bounds;
		float SphereRadius;
		UKismetSystemLibrary::GetComponentBounds(Mesh, Origin, Bounds, SphereRadius);
		const float FixedRadius = Bounds.X - CapsulePadding;
		AttackCapsuleCollision->SetCapsuleHalfHeight(FixedRadius * 2.f);
		AttackCapsuleCollision->SetCapsuleRadius(FixedRadius);		
	}
}

void UKulkiAttributesComponent::SetOwnerSpeed(float& OutMovementSpeed)
{
	const float NewValue = BaseMovementSpeed + (SpeedAttribute.Value * SpeedMultiplier) - (StrengthAttribute.Value * SpeedPenaltyMultiplier);
	OutMovementSpeed = FMath::Clamp(NewValue, MinMovementSpeed, MaxMovementSpeed);
}


