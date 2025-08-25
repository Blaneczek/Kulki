// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KulkiWidgetController.generated.h"

class AKulkiPlayerCharacter;

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
	void SetWidgetControllerParams(AKulkiPlayerCharacter* InPlayerCharacter);

	void InitAttributesValue();
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
	TObjectPtr<AKulkiPlayerCharacter> PlayerCharacter;
	
};
