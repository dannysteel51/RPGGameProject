// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverlapActivateActor.generated.h"

class ARPGCharacter;
class USphereComponent;

UCLASS()
class RPG_API AOverlapActivateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AOverlapActivateActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
	AActor* AttachedLightActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Overlap")
	bool bIsActivated = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Overlap")
	ARPGCharacter* RPGCharacter;
	
protected:
	
	virtual void BeginPlay() override;

private:	

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent;

};
