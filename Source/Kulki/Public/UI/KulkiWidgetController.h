// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KulkiWidgetController.generated.h"

class AKulkiPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, float, NewValue);

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
	FOnAttributeValueChangedSignature OnStrengthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Kulki|Attributes")
	FOnAttributeValueChangedSignature OnSpeedChanged;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Kulki|WidgetController")
	TObjectPtr<AKulkiPlayerCharacter> PlayerCharacter;
};
