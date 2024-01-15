// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ForwardDirectionData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectionDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class RPG_API UForwardDirectionData : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UForwardDirectionData* ForwardDirectionData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FDirectionDataSignature DirectionData;

private:

	virtual void Activate() override;
	void SendForwardDirectionData();
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};

