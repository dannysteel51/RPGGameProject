// Copyright Dan Stull


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ELECTROCUTE </> \n\n"
			
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description - Damage
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing </> "
			"<Damage>%d</> <Default> Lighting damage with "
			"a chance to stun</>"
			),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);	
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ELECTROCUTE </> \n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description - Additional Number of Shock Targets - Damage
			"<Default>Emits beam of lightning, "
			"stringing to %d additional targets nearby, causing </> "
			"<Damage>%d</> <Default> Lightning damage with a chance to stun</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShockTargets - 1),
			ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL </> \n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description - Additional Number of Shock Targets - Damage
			"<Default>Emits beam of lightning, "
			"stringing to %d additional targets nearly, causing </> "
			"<Damage>%d</> <Default> Lightning damage with a chance to stun</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShockTargets - 1),
			ScaledDamage);
}
