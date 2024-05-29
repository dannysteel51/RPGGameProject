// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageToCollectable.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDamageToCollectable : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Parameters", meta=(ExposeOnSpawn = "true"))
	int32 IncomingDamage = 0;
	
};
