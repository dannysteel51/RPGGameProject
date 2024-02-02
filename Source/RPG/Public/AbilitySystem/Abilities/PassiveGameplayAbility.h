// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "PassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPassiveGameplayAbility : public URPGGameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivateAbility(const FGameplayTag& AbilityTag);
};