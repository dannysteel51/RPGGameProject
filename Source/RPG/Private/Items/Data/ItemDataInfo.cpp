// Copyright Dan Stull


#include "Items/Data/ItemDataInfo.h"
#include "GameplayEffect.h"

UGameplayEffect* UItemDataInfo::GetItemEffectClass(FGameplayTag ItemTag) const
{

	for (auto Pair : ItemEffects)
	{
		if (Pair.Key.MatchesTag(ItemTag))
		{
			return Pair.Value;
		}
		return nullptr;
	}
	return nullptr;
}
