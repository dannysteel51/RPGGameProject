// Copyright Dan Stull


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/RPGPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto Pair : GetRPGAttributeSet()->TagsToAttributes)
	{
		// This delegate gets called each time an attributes number value changes
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[Pair, this] (const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	
	GetRPGPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 NewAttributePoints)
		{
			OnPlayerAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
		}
	);
	RPGPlayerState->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 NewSpellPoints)
		{
			OnPlayerSpellPointsChangedDelegate.Broadcast(NewSpellPoints);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	URPGAttributeSet* AS = CastChecked<URPGAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	OnPlayerAttributePointsChangedDelegate.Broadcast(GetRPGPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{;
	GetRPGAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const 
{
	check(AttributeInfo);
	FRPGAttributeInfo Info= AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

