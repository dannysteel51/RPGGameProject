// Copyright Dan Stull


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS </> \n\n"
			
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description - Damage
			"<Default>Summon an Arcane Shard, "
			"causing radial arcane damage of </> "
			"<Damage>%d</> <Default> at the shard origin</>"
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
			"<Title>ARCANE SHARDS </> \n\n"
			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description - Additional Number of Shock Targets - Damage
			"<Default> Summon %d shards of arcane energy, causing radial arcane </> "
			"<Damage>%d</> <Default> damage at the shard origin. </>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
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
			"<Default> Summon %d shards of arcane energy, causing radial arcane </> "
			"<Damage>%d</> <Default> damage at the shard origin. </>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			ScaledDamage);
}
