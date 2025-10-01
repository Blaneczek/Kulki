// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KulkiPlayerController.generated.h"

struct FGameplayTag;
class UKulkiAbilitySystemComponent;
class AKulkiPlayerPawn;
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

	UKulkiAbilitySystemComponent* GetASC();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	
private:
	void StartPlayerInput();
	void StopPlayerInput();
	void AbilityInput(FGameplayTag InputTag);
	
	void FollowMouseCursor();

	
	UPROPERTY(EditAnywhere, Category="Kulki|Input")
	TObjectPtr<UInputMappingContext> KulkiContext;

	UPROPERTY(EditAnywhere, Category="Kulki|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Kulki|Input")
	TObjectPtr<UInputAction> AbilityAction;
	
	UPROPERTY()
	TObjectPtr<AKulkiPlayerPawn> PlayerPawn;

	UPROPERTY()
	TObjectPtr<UKulkiAbilitySystemComponent> PlayerASC;
	
	bool bCanMove;
};
