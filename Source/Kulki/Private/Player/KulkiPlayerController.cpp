// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KulkiPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/KulkiPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AKulkiPlayerController::AKulkiPlayerController()
{
	bCanMove = false;
	PlayerCharacter = nullptr;
}

void AKulkiPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bCanMove)
	{
		FollowMouseCursor();
	}
}

void AKulkiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(KulkiContext, TEXT("AKulkiPlayerController | Kulki Mapping Context is not set"));
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

void AKulkiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PlayerCharacter = CastChecked<AKulkiPlayerCharacter>(InPawn);
}

void AKulkiPlayerController::StartPlayerInput()
{
	bCanMove = true;
}

void AKulkiPlayerController::StopPlayerInput()
{
	bCanMove = false;	
}

void AKulkiPlayerController::FollowMouseCursor()
{
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	// If current velocity is greater than MovementSpeed, stop adding force.
	if (PlayerCharacter->GetMovementComponent()->Velocity.Length() >= PlayerCharacter->GetMovementSpeed())
	{
		return;
	}

	FHitResult HitResult;
	// ECC_GameTraceChannel1 - Floor
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);	
	if (HitResult.bBlockingHit)
	{
		const float MovementForce = PlayerCharacter->GetMovementForce();
		const FVector HitDirection = (HitResult.ImpactPoint - PlayerCharacter->GetActorLocation()).GetSafeNormal();
		const FVector Force = FVector(HitDirection.X * MovementForce, HitDirection.Y * MovementForce, 0.f);
		PlayerCharacter->GetCharacterMovement()->AddForce(Force);
	}
}




