// Copyright Dan Stull


#include "Characters/RPGAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/RPGCharacter.h"
#include "Components/CustomMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(!Character || !CustomMovementComponent) return;
	GetGroundSpeed();
	GetAirSpeed();
	GetShouldMove();
	GetIsFalling();
	GetCharVelocity();
	GetDirection();
	GetIsSwimming();
	GetIsClimbing();
	GetClimbVelocity();
	GetInShockLoop();
	GetInBlockLoop();
	GetInBowShotLoop();
}

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ARPGCharacter>(TryGetPawnOwner());

	if (Character)
	{
		CustomMovementComponent = Cast<UCustomMovementComponent>(Character->GetCharacterMovement());
	}
}

void URPGAnimInstance::GetGroundSpeed()
{
	GroundSpeed = UKismetMathLibrary::VSizeXY(Character->GetVelocity());
}

void URPGAnimInstance::GetAirSpeed()
{
	AirSpeed = Character->GetVelocity().Z;
}

void URPGAnimInstance::GetShouldMove()
{
	bShouldMove =
		CustomMovementComponent->GetCurrentAcceleration().Size() > 0 &&
			GroundSpeed > 5.f &&
				!bIsFalling;
}

void URPGAnimInstance::GetIsFalling()
{
	bIsFalling = CustomMovementComponent->IsFalling();
}

void URPGAnimInstance::GetCharVelocity()
{
	Velocity = Character->GetVelocity();
}

void URPGAnimInstance::GetDirection()
{
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());
}

void URPGAnimInstance::GetIsSwimming()
{
	bIsSwimming = Character->GetCharacterMovement()->IsSwimming();
}

void URPGAnimInstance::GetIsClimbing()
{
	bIsClimbing = CustomMovementComponent->IsClimbing();
}

void URPGAnimInstance::GetClimbVelocity()
{
	ClimbVelocity = CustomMovementComponent->GetUnRotatedClimbVelocity();
}

void URPGAnimInstance::GetInShockLoop()
{
	bInShockLoop = Character->GetInShockLoop();
}

void URPGAnimInstance::GetInBlockLoop()
{
	bInBlockLoop = Character->GetInBlockLoop();
}

void URPGAnimInstance::GetInBowShotLoop()
{
	bInBowShotLoop = Character->GetInBowShotLoop();
}

