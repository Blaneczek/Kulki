// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KulkiPlayerController.generated.h"

class AKulkiPlayerCharacter;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class KULKI_API AKulkiPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AKulkiPlayerController();

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	
private:
	void StartPlayerInput();
	void StopPlayerInput();

	void FollowMouseCursor();
	
	UPROPERTY(EditAnywhere, Category="Kulki|Input")
	TObjectPtr<UInputMappingContext> KulkiContext;

	UPROPERTY(EditAnywhere, Category="Kulki|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY()
	TObjectPtr<AKulkiPlayerCharacter> PlayerCharacter;
	
	bool bCanMove;
};
