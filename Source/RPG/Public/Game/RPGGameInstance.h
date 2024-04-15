// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	bool AlreadySetActorLocation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	FVector PlayerEntranceLocation = FVector(0.0f, 0.0f, 0.0f);
};
