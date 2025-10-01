// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiBasePawn.h"
#include "Gameplay/KulkiCombatInterface.h"
#include "KulkiEnemyPawn.generated.h"

DECLARE_DELEGATE(FOnCheckIfBigger);
DECLARE_DELEGATE(FOnBackToIdle);

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	RED		UMETA(DisplayName = "RED"),
	YELLOW	UMETA(DisplayName = "YELLOW"),
	PURPLE	UMETA(DisplayName = "PURPLE")
};

/**
 * 
 */
UCLASS()
class KULKI_API AKulkiEnemyPawn : public AKulkiBasePawn, public IKulkiCombatInterface
{
	GENERATED_BODY()

public:
	AKulkiEnemyPawn();

	void SetOverlapGameplayEffectClass(TSubclassOf<UGameplayEffect> GameplayEffectClass);

	void InitSpawn(float Strength, float Speed);
	
	/* IKulkiCombatInterface */
	virtual void ApplyOverlapEffect(UAbilitySystemComponent* TargetASC, float Coefficient, bool& OutIsEatableEnemy) override;
	virtual float GetStrength() const override;
	/* */

	/* Gets new overlapping actors for avoidance system. */
	void ResetNeighbors();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	EEnemyType Type = EEnemyType::NONE;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Kulki")
	bool bIdleState = true;

	FOnCheckIfBigger OnCheckIfBigger;
	FOnBackToIdle OnBackToIdle;

	/* Other Enemies that overlap with this pawn. */
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> AvoidanceNeighbors;
		
protected:	
	virtual void BeginPlay() override;

	/* Gameplay Effect used to set default values of attributes. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Kulki|Attributes")
    TSubclassOf<UGameplayEffect> SpawnAttributes;

	/* GameplayEffect used when overlapping with Player. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Kulki")
	TSubclassOf<UGameplayEffect> OverlapGameplayEffectClass;

	/* Overlaps with Player. Sphere for checking if AI state should change to Chase or Escape. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> StartAICheckSphere;

	/* Overlaps with Player. Sphere for going back to Idle state.  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> EndAICheckSphere;

	/* Overlaps with other Enemies. Sphere for adding neighbor for avoidance system. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> StartAvoidanceSphere;

	/* Overlaps with other Enemies. Sphere for removing neighbor for avoidance system. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> EndAvoidanceSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	float StartAICheckRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	float EndAICheckRadius;
	
private:
	void SetMeshColor();
	void SetAISpheresSize();
	void SetAttributesValue(float Strength, float Speed);
	
	UFUNCTION()
	void OnStartAICheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndAICheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnStartAvoidance(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndAvoidance(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
