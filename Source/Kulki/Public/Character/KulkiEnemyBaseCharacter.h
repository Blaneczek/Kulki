// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KulkiEnemyBaseCharacter.generated.h"

class UCapsuleComponent;
class UKulkiAttributesComponent;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	NONE	UMETA(DisplayName = "NONE"),
	RED		UMETA(DisplayName = "RED"),
	YELLOW	UMETA(DisplayName = "YELLOW"),
	PURPLE	UMETA(DisplayName = "PURPLE")
};

UCLASS()
class KULKI_API AKulkiEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKulkiEnemyBaseCharacter();
	
	UFUNCTION(BlueprintCallable)
	UKulkiAttributesComponent* GetAttributesComponent() const { return AttributesComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DebugStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DebugSpeed;
	
protected:
	virtual void BeginPlay() override;

	void SetMeshColor();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UKulkiAttributesComponent> AttributesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CapsuleCollision;
	
private:
	UPROPERTY()
	FLinearColor MeshColor = FLinearColor::Gray;

	
};
