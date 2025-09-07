// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KulkiBasePawn.h"
#include "KulkiEnemyPawn.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	RED		UMETA(DisplayName = "RED"),
	YELLOW	UMETA(DisplayName = "YELLOW"),
	PURPLE	UMETA(DisplayName = "PURPLE")
};

UENUM(BlueprintType, Blueprintable)
enum class EEnemyState: uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	IDLE	UMETA(DisplayName = "IDLE"),
	CHASE	UMETA(DisplayName = "CHASE"),
	ESCAPE	UMETA(DisplayName = "ESCAPE")
};

/**
 * 
 */
UCLASS()
class KULKI_API AKulkiEnemyPawn : public AKulkiBasePawn
{
	GENERATED_BODY()

public:
	AKulkiEnemyPawn();

	void SetSpawnAttributesValue(float Strength, float Speed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Kulki")
	void SetState(EEnemyState NewState);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Kulki")
	EEnemyType Type = EEnemyType::NONE;
	
	UPROPERTY(BlueprintReadOnly, Category="Kulki")
	bool bCanChase = true;
	
protected:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Kulki|Attributes")
	TSubclassOf<UGameplayEffect> SpawnAttributes;
	
	virtual void BeginPlay() override;

	void SetMeshColor();
	
};
