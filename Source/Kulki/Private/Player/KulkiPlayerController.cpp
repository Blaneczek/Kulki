// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KulkiPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/KulkiPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnitConversion.h"

AKulkiPlayerController::AKulkiPlayerController()
{
	bIsMoving = false;
}

void AKulkiPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bIsMoving)
	{
		FollowMouseCursor();
	}
}

void AKulkiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(KulkiContext)

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(KulkiContext, 0);
	}

	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AKulkiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AKulkiPlayerController::StartPlayerInput);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AKulkiPlayerController::StopPlayerInput);
}

void AKulkiPlayerController::StartPlayerInput()
{
	bIsMoving = true;
}

void AKulkiPlayerController::StopPlayerInput()
{
	bIsMoving = false;	
	if (AKulkiPlayerCharacter* PlayerCharacter = Cast<AKulkiPlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
	}
}

void AKulkiPlayerController::FollowMouseCursor()
{
	//TODO: maybe try something different 
	AKulkiPlayerCharacter* PlayerCharacter = Cast<AKulkiPlayerCharacter>(GetCharacter());
	if (!PlayerCharacter)
	{
		return;
	}

	if (PlayerCharacter->GetMovementComponent()->Velocity.Length() >= PlayerCharacter->GetMovementSpeed())
	{
		return;
	}
	
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);	
	if (HitResult.bBlockingHit)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - PlayerCharacter->GetActorLocation()).GetSafeNormal();
		const FVector Force = FVector(HitDirection.X * PlayerCharacter->GetMovementForce(), HitDirection.Y * PlayerCharacter->GetMovementForce(), 0.f);
		PlayerCharacter->GetCharacterMovement()->AddForce(Force);
	}
}




