// Copyright Dan Stull


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"
#include "Player/RPGPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnPlayerSpellPointsChangedDelegate.Broadcast(GetRPGPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetRPGAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				FString DescriptionString;
				FString NextLevelDescriptionString;
				GetRPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, DescriptionString, NextLevelDescriptionString);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, DescriptionString, NextLevelDescriptionString);
			}
			
			if (AbilityInfo)
			{
				FRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetRPGAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	GetRPGPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			OnPlayerSpellPointsChangedDelegate.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;

			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString DescriptionString;
			FString NextLevelDescriptionString;
			GetRPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, DescriptionString, NextLevelDescriptionString);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, DescriptionString, NextLevelDescriptionString);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();
	const int32 SpellPoints = GetRPGPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetRPGAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetRPGAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString DescriptionString;
	FString NextLevelDescriptionString;
	GetRPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, DescriptionString, NextLevelDescriptionString);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, DescriptionString, NextLevelDescriptionString);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetRPGAbilitySystemComponent())
	{
		GetRPGAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.Ability = FRPGGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FRPGGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetRPGAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if(SelectedStatus.MatchesTagExact(FRPGGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetRPGAbilitySystemComponent()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	// Check selected ability against slot's ability type.(Don't equip a passive to offensive slot and vice versa)
	const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetRPGAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag)
{
	bWaitingForEquipSelection = false;

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

	StopWaitingForEquipSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
