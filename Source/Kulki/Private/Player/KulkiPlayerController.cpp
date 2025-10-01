// Copyright (c) 2025 Dawid Szoldra. All rights reserved.


#include "Player/KulkiPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/AbilitySystem/KulkiAbilitySystemComponent.h"
#include "Character/KulkiPlayerPawn.h"
#include "Gameplay/KulkiGameplayTags.h"

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

UKulkiAbilitySystemComponent* AKulkiPlayerController::GetASC()
{
	if (!PlayerASC)
	{
		PlayerASC = Cast<UKulkiAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return PlayerASC;
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
	EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &AKulkiPlayerController::AbilityInput, KulkiGameplayTags::Input_Spacebar.GetTag());
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

void AKulkiPlayerController::AbilityInput(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagHeld(InputTag);
	}
}

void AKulkiPlayerController::FollowMouseCursor()
{
	if (!PlayerPawn)
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




