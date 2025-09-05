// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KulkiPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/KulkiPlayerPawn.h"

AKulkiPlayerController::AKulkiPlayerController()
{
	bCanMove = false;
	PlayerPawn = nullptr;
}

void AKulkiPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

	PlayerPawn = CastChecked<AKulkiPlayerPawn>(InPawn);
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
	if (!IsValid(PlayerPawn))
	{
		return;
	}

	FHitResult HitResult;
	// ECC_GameTraceChannel1 - Floor
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);	
	if (HitResult.bBlockingHit)
	{		
		const FVector HitDirection = (HitResult.ImpactPoint - PlayerPawn->GetActorLocation()).GetSafeNormal();
		PlayerPawn->AddMovementInput(FVector(HitDirection.X, HitDirection.Y,0.f));
	}
}




