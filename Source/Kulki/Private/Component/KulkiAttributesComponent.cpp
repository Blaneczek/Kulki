// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/KulkiAttributesComponent.h"

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


