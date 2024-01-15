// Copyright Dan Stull


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForExperience(int32 Experience) const
{

	int32 Level = 1;
	bool bSearching = true;
	while(bSearching)
	{
		// Level up information[1] is level 1 information
		// Level up information[2] is level 1 information
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		if (Experience >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
