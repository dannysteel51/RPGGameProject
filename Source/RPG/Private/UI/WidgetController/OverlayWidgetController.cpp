// Copyright Dan Stull


#include "UI/WidgetController/OverlayWidgetController.h"

#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/RPGPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetRPGAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetRPGAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetRPGAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetRPGAttributeSet()->GetMaxMana());
}


void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetRPGPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	RPGPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetRPGAttributeSet()->GetHealthAttribute()).AddLambda(
	[this] (const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetRPGAttributeSet()->GetMaxHealthAttribute()).AddLambda(
	[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetRPGAttributeSet()->GetManaAttribute()).AddLambda(
	[this ] (const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetRPGAttributeSet()->GetMaxManaAttribute()).AddLambda(
	[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (GetRPGAbilitySystemComponent())
	{
		GetRPGAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		if (GetRPGAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetRPGAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		GetRPGAbilitySystemComponent()->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// For example, say that Tag = Message.HealthPotion
				// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRow.Broadcast(*Row);
				}
			}
		}
	);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) 
{
	const ULevelUpInfo* LevelUpInfo = GetRPGPlayerState()->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to find level up info/ Please fill out RPGPlayerState.LevelUpInfo"));

	const int32 Level = LevelUpInfo->FindLevelForExperience(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		// Level 1 is 300 xp
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		// Level 0 is 0 xp
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		// Delta Level Requirement is the difference between the current level requirement and the previous level requirement
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		
		// XP for this level for level up is the difference between the current XP and the previous level requirement
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		// XP Bar Percent is the percentage of XP for this level divided by the delta level requirement
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag) const
{
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
	
	FRPGAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = FRPGGameplayTags::Get().Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlotTag;
	LastSlotInfo.AbilityTag = FRPGGameplayTags::Get().Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);
}
