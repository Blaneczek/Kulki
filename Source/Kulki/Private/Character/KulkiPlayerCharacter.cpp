// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KulkiPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AKulkiPlayerCharacter::AKulkiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>("CameraArm");
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetUsingAbsoluteRotation(true);
	CameraArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
}

void AKulkiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKulkiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


