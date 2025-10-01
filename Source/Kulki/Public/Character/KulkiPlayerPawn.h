// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiBasePawn.h"
#include "KulkiPlayerPawn.generated.h"

class UKulkiCameraComponent;
class USpringArmComponent;

DECLARE_MULTICAST_DELEGATE(FOnImmunityActivation);
DECLARE_MULTICAST_DELEGATE(FOnImmunityDeactivation);
DECLARE_DELEGATE(FOnEatableEnemyKilled);
DECLARE_MULTICAST_DELEGATE(FOnStrengthChanged);

/**
 * 
 */
UCLASS()
class KULKI_API AKulkiPlayerPawn : public AKulkiBasePawn
{
	GENERATED_BODY()

public:
	AKulkiPlayerPawn();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;
	
	bool IsImmune() const { return bIsImmune; }
	
	FOnImmunityActivation OnImmunityActivation;
	FOnImmunityDeactivation OnImmunityDeactivation;
	FOnEatableEnemyKilled OnEatableEnemyKilled;
	FOnStrengthChanged OnStrengthChanged;
	
protected:
	virtual void BeginPlay() override;

	virtual void AddAbilities() override;
	virtual void SetKulkiPawnSize(const FOnAttributeChangeData& Data) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<UKulkiCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kulki")
	TObjectPtr<USpringArmComponent> CameraArm;

	/* Time during which Player can't be hit. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(UIMin="0.1", ClampMin="0.1"), Category="Kulki|Immunity")
	float ImmunityTime = 2.f;

	/* Mesh color during the immune state. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Kulki|Immunity")
	FLinearColor ImmunityColor;

	/* Gameplay Effect used to set default values of attributes. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Kulki|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	UPROPERTY(EditAnywhere, Category="Kulki|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnPlayerLost();
	
	void ActivateImmunity();
	void DeactivateImmunity(const FLinearColor Color);

	void EnemyHitApplyEffectToSelf(APawn* Enemy, TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, float Coefficient);

	void InitDefaultAttributes();
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
	bool bIsImmune = false;
};
