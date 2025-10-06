// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Component/KulkiCameraComponent.h"

UKulkiCameraComponent::UKulkiCameraComponent()
{
	ProjectionMode = ECameraProjectionMode::Orthographic;
	OrthoWidth = 3000.f;
}

void UKulkiCameraComponent::ChangeOrtoWidth(float Strength)
{
	if (Strength >= PlayerStrengthChangeStep * (StepNumber + 1))
	{
		StepNumber++;
		OrthoWidthOld = OrthoWidth;
		OrthoWidthNew = OrthoWidth + OrthoWidthChangeStep;
	}
	else if (StepNumber > 0 && Strength < (PlayerStrengthChangeStep * StepNumber) - 30.f)
	{
		StepNumber--;
		OrthoWidthOld = OrthoWidth;
		OrthoWidthNew = OrthoWidth - OrthoWidthChangeStep;
	}
	else
	{
		// return without changing camera 
		return;
	}

	FOnTimelineFloat TimelineProgress;
    TimelineProgress.BindUFunction(this, FName("UpdateWidth"));
    OrthoWidthTimeline.AddInterpFloat(OrthoWidthChangeCurve, TimelineProgress);
	OrthoWidthTimeline.PlayFromStart();
}

void UKulkiCameraComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OrthoWidthTimeline.TickTimeline(DeltaTime);
}

void UKulkiCameraComponent::UpdateWidth(float Alpha)
{
	const float NewWidth = FMath::Lerp(OrthoWidthOld, OrthoWidthNew, Alpha);
	OrthoWidth = NewWidth;
}


