// Copyright Dan Stull


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}

FCharacterClassDefaultInfo UCharacterClassInfo::SetClassDefaultInfo(ECharacterClass CharacterClass,
	FCharacterClassDefaultInfo ClassDefaultInfo)
{
	return CharacterClassInformation.Add(CharacterClass, ClassDefaultInfo);
}
