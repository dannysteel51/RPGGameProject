// Copyright Dan Stull


#include "AbilitySystem/Data/AttributeInfo.h"

#include "RPG/RPGLogChannels.h"

FRPGAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FRPGAttributeInfo& Info : AttributeInfo)
	{
		if (Info.AttributeTag.MatchesTagExact(Tag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		//UE_LOG(LogRPG, Warning, TEXT("UAttributeInfo::FindAttributeInfoForTag - AttributeInfo for Tag [%s] not found on AttributeInfo [%s]."), *Tag.ToString(), *GetNameSafe(this));
		UE_LOG(LogTemp, Error, TEXT("AbilityTag [%s] on Abilityinfor [%s] not found"), *Tag.ToString(), *GetNameSafe(this));
	}
	return FRPGAttributeInfo();
}
