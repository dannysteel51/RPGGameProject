// Copyright Dan Stull


#include "AbilitySystem/Data/AbilityInfo.h"

#include "RPG/RPGLogChannels.h"

FRPGAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for (auto AbilityInfo : AbilityInformation)
	{
		if (AbilityInfo.AbilityTag == Tag)
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogRPG, Error, TEXT("AbilityTag [%s] on Abilityinfor [%s] not found"), *Tag.ToString(), *GetNameSafe(this));
	}
	
	return FRPGAbilityInfo();
}
