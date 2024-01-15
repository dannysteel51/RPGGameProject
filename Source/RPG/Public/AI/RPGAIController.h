// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGAIController.generated.h"

class UBehaviorTreeComponent;


/**
 * 
 */
UCLASS()
class RPG_API ARPGAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARPGAIController();
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
