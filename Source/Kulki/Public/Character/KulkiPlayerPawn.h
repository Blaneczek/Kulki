// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiBasePawn.h"
#include "KulkiPlayerPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_DELEGATE(FOnImmunityActivation);
DECLARE_DELEGATE(FOnImmunityDeactivation);
DECLARE_DELEGATE(FOnEatableEnemyKilled);

/**
 * 
 */
UCLASS()
class KULKI_API AKulkiPlayerPawn : public AKulkiBasePawn
{
	GENERATED_BODY()

public:
	AKulkiPlayerPawn();
	
	FOnImmunityActivation OnImmunityActivation;
	FOnImmunityDeactivation OnImmunityDeactivation;
	FOnEatableEnemyKilled OnEatableEnemyKilled;
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<USpringArmComponent> CameraArm;

	/* Time during which Player can't be hit. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(UIMin="0.1", ClampMin="0.1"), Category="Kulki|Immunity")
	float ImmunityTime = 2.f;

	/* Mesh color during the immune state. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Kulki|Immunity")
	FLinearColor ImmunityColor;

private:
	UFUNCTION()
	void OnOverlapAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnPlayerLost();
	
	void ActivateImmunity();
	
	void DeactivateImmunity(const FLinearColor Color);

	void EnemyHitApplyEffectToSelf(APawn* Enemy, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, float Coefficient);
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
	bool bIsImmune = false;
};
