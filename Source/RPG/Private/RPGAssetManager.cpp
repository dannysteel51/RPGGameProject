// Copyright Dan Stull


#include "RPGAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "RPGGameplayTags.h"

URPGAssetManager& URPGAssetManager::Get()
{
	check(GEngine);
	URPGAssetManager* RPGAssetManager = Cast<URPGAssetManager>(GEngine->AssetManager);
	return *RPGAssetManager;
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FRPGGameplayTags::InitializeNativeGameplayTags();

	// Required to use target data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
