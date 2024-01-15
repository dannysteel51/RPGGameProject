// Copyright Dan Stull


#include "UI/WidgetController/RPGWidgetController.h"

#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/RPGPlayerController.h"
#include "Player/RPGPlayerState.h"

void URPGWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void URPGWidgetController::BroadcastInitialValues()
{
}

void URPGWidgetController::BindCallbacksToDependencies()
{
}

void URPGWidgetController::BroadcastAbilityInfo()
{
	if (!GetRPGAbilitySystemComponent()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(RPGAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = RPGAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = RPGAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	RPGAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

ARPGPlayerController* URPGWidgetController::GetRPGPlayerController()
{
	if (RPGPlayerController == nullptr)
	{
		RPGPlayerController = Cast<ARPGPlayerController>(PlayerController);
	}
	return RPGPlayerController;
}

ARPGPlayerState* URPGWidgetController::GetRPGPlayerState() 
{
	if (RPGPlayerState == nullptr)
	{
		RPGPlayerState = Cast<ARPGPlayerState>(PlayerState);
	}
	return RPGPlayerState;
}

URPGAbilitySystemComponent* URPGWidgetController::GetRPGAbilitySystemComponent() 
{
	if (RPGAbilitySystemComponent == nullptr)
	{
		RPGAbilitySystemComponent = Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
	}
	return RPGAbilitySystemComponent;
}

URPGAttributeSet* URPGWidgetController::GetRPGAttributeSet() 
{
	if (RPGAttributeSet == nullptr)
	{
		RPGAttributeSet = Cast<URPGAttributeSet>(AttributeSet);
	}
	return RPGAttributeSet;
}
