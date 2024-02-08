// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class RPG_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
};
