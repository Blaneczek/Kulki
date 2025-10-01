// Copyright (c) 2025 Dawid Szoldra. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiBasePawn.h"
#include "Gameplay/KulkiCombatInterface.h"
#include "KulkiEnemyPawn.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCheckIfBigger);
DECLARE_MULTICAST_DELEGATE(FOnBackToIdle);

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

	void SetSpawnAttributesValue(float Strength, float Speed);	
	void SetSpawnOverlapGameplayEffectClass(TSubclassOf<UGameplayEffect> GameplayEffectClass);

	/* IKulkiCombatInterface */
	virtual void ApplyOverlapEffect(UAbilitySystemComponent* TargetASC, float Coefficient, bool& OutIsEatableEnemy) override;
	virtual float GetStrength() const override;
	/* */
	
	void ResetNeighbours();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	EEnemyType Type = EEnemyType::NONE;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kulki")
	bool bIdleState = true;

	FOnCheckIfBigger OnCheckIfBigger;
	FOnBackToIdle OnBackToIdle;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> AvoidanceNeighbours;
		
protected:	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Kulki|Attributes")
    TSubclassOf<UGameplayEffect> SpawnAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Kulki")
	TSubclassOf<UGameplayEffect> OverlapGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> StartAICheckSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> EndAICheckSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> StartAvoidanceSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	TObjectPtr<USphereComponent> EndAvoidanceSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	float StartAICheckRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	float EndAICheckRadius;
	
private:
	void SetMeshColor();
	void SetAISpheresSize();

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
