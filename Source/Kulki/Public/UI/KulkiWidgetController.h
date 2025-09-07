// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KulkiWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class AKulkiPlayerPawn;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, APawn* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		:PlayerController(PC), PlayerPawn(PS), AbilitySystemComponent(ASC), AttributeSet(AS)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> PlayerPawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/* Delegate for communication with the widget. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChanged, float, NewValue);

/**
 * 
 */
UCLASS(Blueprintable)
class KULKI_API UKulkiWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	void BroadcastInitialValues();
	void BindCallbacks();
	
	UPROPERTY(BlueprintAssignable, Category = "Kulki|Attributes")
	FOnAttributeValueChanged OnStrengthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Kulki|Attributes")
	FOnAttributeValueChanged OnMaxStrengthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Kulki|Attributes")
	FOnAttributeValueChanged OnSpeedChanged;

	UPROPERTY(BlueprintAssignable, Category = "Kulki|Attributes")
	FOnAttributeValueChanged OnMaxSpeedChanged;
		
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Kulki|WidgetController")
	float MaxStrength;

	UPROPERTY(BlueprintReadOnly, Category = "Kulki|WidgetController")
	float MaxSpeed;

private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY()
	TObjectPtr<APawn> PlayerPawn;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
