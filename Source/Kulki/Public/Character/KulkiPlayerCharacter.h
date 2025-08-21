// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KulkiPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class KULKI_API AKulkiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKulkiPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float DebugSpeed = 100.f;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraArm;
	
};
