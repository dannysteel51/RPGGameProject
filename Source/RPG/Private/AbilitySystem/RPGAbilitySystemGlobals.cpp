// Copyright Dan Stull


#include "AbilitySystem/RPGAbilitySystemGlobals.h"

#include "RPGAbilitySystemTypes.h"

FGameplayEffectContext* URPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRPGGameplayEffectContext();
}
