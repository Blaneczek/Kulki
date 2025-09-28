// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/KulkiCameraComponent.h"

UKulkiCameraComponent::UKulkiCameraComponent()
{
	ProjectionMode = ECameraProjectionMode::Orthographic;
	OrthoWidth = 3000.f;
	StepNumber = 0;
}

void UKulkiCameraComponent::ChangeOrtoWidth(float Strength)
{
	if (Strength >= PlayerStrengthChangeStep * (StepNumber + 1))
	{
		StepNumber++;
		OrthoWidth += OrthoWidthChangeStep;
	}
	else if (StepNumber > 0 && Strength < (PlayerStrengthChangeStep * StepNumber) - 30.f)
	{
		StepNumber--;
		OrthoWidth -= OrthoWidthChangeStep;
	}
}


