// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RPGWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayTag;
struct FGameplayAttribute;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInforSignature, const FRPGAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_API UAttributeMenuWidgetController : public URPGWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "Gas|Attributes")
	FAttributeInforSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Gas|Attributes")
	FOnPlayerStatChanged OnPlayerAttributePointsChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Gas|Attributes")
	FOnPlayerStatChanged OnPlayerSpellPointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

	
};
