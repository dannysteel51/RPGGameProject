// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGAnimInstance.generated.h"

class UCustomMovementComponent;
class ARPGCharacter;
/**
 * 
 */
UCLASS()
class RPG_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	ARPGCharacter* Character;

	UPROPERTY()
	UCustomMovementComponent* CustomMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
	void GetGroundSpeed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float AirSpeed;
	void GetAirSpeed();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bShouldMove;
	void GetShouldMove();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	void GetIsFalling();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;
	void GetCharVelocity();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float Direction;
	void GetDirection();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsSwimming;
	void GetIsSwimming();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsClimbing;
	void GetIsClimbing();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	FVector ClimbVelocity;
	void GetClimbVelocity();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bInShockLoop;
	void GetInShockLoop();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bInBlockLoop;
	void GetInBlockLoop();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bInBowShotLoop;
	void GetInBowShotLoop();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsStunned;
	void GetIsStunned();
};

