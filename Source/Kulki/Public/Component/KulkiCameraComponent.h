// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "KulkiCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class KULKI_API UKulkiCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UKulkiCameraComponent();
	
	void ChangeOrtoWidth(float Strength);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kulki")
	float OrthoWidthChangeStep = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kulki")
	float PlayerStrengthChangeStep = 150.f;

private:
	uint8 StepNumber = 0;
};
