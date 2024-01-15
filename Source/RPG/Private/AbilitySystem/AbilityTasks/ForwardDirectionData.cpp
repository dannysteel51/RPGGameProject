// Copyright Dan Stull


#include "AbilitySystem/AbilityTasks/ForwardDirectionData.h"

#include "AbilitySystemComponent.h"
#include "Player/RPGPlayerController.h"

UForwardDirectionData* UForwardDirectionData::ForwardDirectionData(UGameplayAbility* OwningAbility)
{
	UForwardDirectionData* MyObj = NewAbilityTask<UForwardDirectionData>(OwningAbility);
	return MyObj;
}

void UForwardDirectionData::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendForwardDirectionData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UForwardDirectionData::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UForwardDirectionData::SendForwardDirectionData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	ARPGPlayerController* PlayerController = Cast<ARPGPlayerController>(PC);
	FHitResult HitResult;
	FHitResult TraceHit = PlayerController->LineTraceForSpells(HitResult);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = TraceHit;
	DataHandle.Add(Data);

	FGameplayTag ApplicationTag;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		ApplicationTag,
		AbilitySystemComponent->ScopedPredictionKey);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		DirectionData.Broadcast(DataHandle);
	}
}

void UForwardDirectionData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		DirectionData.Broadcast(DataHandle);
	}
}




