// Copyright Dan Stull


#include "Player/RPGPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/CustomMovementComponent.h"
#include "GameFramework/Character.h"
#include "Input/RPGInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/RPG.h"
#include "UI/Widget/DamageTextComponent.h"


ARPGPlayerController::ARPGPlayerController()
{
	bReplicates = true;
	bMenuVisible = false;
	bTabDown = false;
	//bIsInAir = GetCharacter()->GetCharacterMovement()->IsFalling();
	PlayerController = nullptr;

}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	checkf(RPGContext, TEXT("RPGContext is null in ARPGPlayerController::BeginPlay(), set on Playstate")); // Halts execution if RPGContext is null
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(RPGContext, 0);
	}
	checkf(AttributePauseMenuClass, TEXT("AttributePauseMenuClass is null in ARPGPlayerController blueprint"));
	checkf(SpellPauseMenuClass, TEXT("SpellPauseMenuClass is null in ARPGPlayerController blueprint"));
	
	if (AttributePauseMenuClass && SpellPauseMenuClass)
	{
		AttributePauseMenu = CreateWidget<UUserWidget>(this, AttributePauseMenuClass);
		SpellPauseMenu = CreateWidget<UUserWidget>(this, SpellPauseMenuClass);
        	if (AttributePauseMenu && SpellPauseMenu)
        	{
        		AttributePauseMenu->AddToViewport();
        		AttributePauseMenu->SetPositionInViewport(FVector2d(85.f, 25.f));
        		AttributePauseMenu->SetVisibility(ESlateVisibility::Hidden);

        		SpellPauseMenu->AddToViewport();
        		SpellPauseMenu->SetPositionInViewport(FVector2d(475.f, 25.f));
        		SpellPauseMenu->SetVisibility(ESlateVisibility::Hidden);
        	}
	}
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	URPGInputComponent* RPGInputComponent = CastChecked<URPGInputComponent>(InputComponent);
	RPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Move);
	RPGInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Look);
	RPGInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &ARPGPlayerController::TabDown);
	RPGInputComponent->BindAction(PauseAction, ETriggerEvent::Canceled, this, &ARPGPlayerController::TabUp);
	RPGInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ARPGPlayerController::ClimbActionStarted);

	/*
	 * These are implemented in Blueprint now
	 */
	
	//RPGInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Dodge);
	//RPGInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Run);
	//RPGInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ARPGPlayerController::RunStop);
	//RPGInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ARPGPlayerController::Select);
	//RPGInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Jump);
	
	RPGInputComponent->BindAbilityToActions(InputConfig, this, &ARPGPlayerController::AbilityInputTagPressed, &ARPGPlayerController::AbilityInputTagReleased, &ARPGPlayerController::AbilityInputTagHeld);
}

void ARPGPlayerController::Move(const FInputActionValue& Value)
{
	if(bTabDown) return;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	UCustomMovementComponent* CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacter()->GetCharacterMovement());
	if(!CustomMovementComponent) return;
	if(CustomMovementComponent->IsClimbing())
	{
		HandleClimbMovementInput(Value);
	}
	else
	{
		HandleGroundMovementInput(Value);
	}
}

void ARPGPlayerController::HandleGroundMovementInput(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (ARPGCharacter* PlayerCharacter = Cast<ARPGCharacter>(GetCharacter()))
	{
		if (PlayerCharacter->bIsInWater)
		{
			const FVector CameraForwardDirection = PlayerCharacter->GetCameraComponent()->GetForwardVector();
			PlayerCharacter->AddMovementInput(CameraForwardDirection, MovementVector.Y);
			PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
		}
		else
		{
			PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
			PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ARPGPlayerController::HandleClimbMovementInput(const FInputActionValue& Value)
{
	UCustomMovementComponent* CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacter()->GetCharacterMovement());
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector ForwardDirection = FVector::CrossProduct(
	-CustomMovementComponent->GetClimbableSurfaceNormal(), GetCharacter()->GetActorRightVector()
	);

	const FVector RightDirection = FVector::CrossProduct(
	CustomMovementComponent->GetClimbableSurfaceNormal(), GetCharacter()->GetActorUpVector()
	);

	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ARPGPlayerController::Look(const FInputActionValue& Value)
{
		if (bTabDown) return;
		if(bIsTargeting)
		{
			const FVector2D LookAxisVector = Value.Get<FVector2D>();
			const FVector2D NewLookAxisVector = LookAxisVector * 0.2f;
			GetCharacter()->AddControllerPitchInput(NewLookAxisVector.Y);
		}
		else
		{
			const FVector2D LookAxisVector = Value.Get<FVector2D>();
			GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);
			GetCharacter()->AddControllerYawInput(LookAxisVector.X);
		}
}

void ARPGPlayerController::Jump()
{
	if (bTabDown || bIsCharacterAttacking) return;
	GetCharacter()->Jump();
}


void ARPGPlayerController::ClimbActionStarted(const FInputActionValue& Value)
{
	UCustomMovementComponent* CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacter()->GetCharacterMovement());
	if(!CustomMovementComponent) return;
	if(!CustomMovementComponent->IsClimbing())
	{
		CustomMovementComponent->ToggleClimbing(true);
	}
	else
	{
		CustomMovementComponent->ToggleClimbing(false);
	}
}

void ARPGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (bTabDown) return;
	//bIsCharacterAttacking = true;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ARPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (bTabDown) return;
	//bIsCharacterAttacking = false;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void ARPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (bTabDown) return;
	//bIsCharacterAttacking = true;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FRPGGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

FHitResult ARPGPlayerController::LineTraceForSpells(FHitResult &OutHitResult) const 
{
	FVector2d ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CenterScreen(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	//CenterScreen.Y -= 50.f;
	FVector CameraLocation;
	FVector CameraDirection;
            			
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(this, CenterScreen, CameraLocation, CameraDirection);
	if (bScreenToWorld)
	{
		const FVector Start = CameraLocation;
		const FVector End = Start + CameraDirection * 50'000.f;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Target);
	}
	return OutHitResult;
}

void ARPGPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlocked, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent(); // Needed when not creating component in the constructor
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlocked, bCriticalHit);
	}
}

URPGAbilitySystemComponent* ARPGPlayerController::GetASC()
{
	if (RPGAbilitySystemComponent == nullptr)
	{
		RPGAbilitySystemComponent = Cast<URPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return RPGAbilitySystemComponent;
}

void ARPGPlayerController::PauseGame_Implementation()
{
	bTabDown = true;
	bMenuVisible = true;
	bShowMouseCursor = true;
	const FInputModeGameAndUI InputModeData;
	SetInputMode(InputModeData);
	AttributePauseMenu->SetVisibility(ESlateVisibility::Visible);
	SpellPauseMenu->SetVisibility(ESlateVisibility::Visible);
}

void ARPGPlayerController::UnpauseGame_Implementation()
{
	bTabDown = false;
	bMenuVisible = false;
	bShowMouseCursor = false;
	const FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	AttributePauseMenu->SetVisibility(ESlateVisibility::Hidden);
	SpellPauseMenu->SetVisibility(ESlateVisibility::Hidden);
}

void ARPGPlayerController::TabDown()
{
	bTabDown = true;
	TogglePauseMenu();
}

void ARPGPlayerController::TabUp()
{
	bTabDown = false;
}

void ARPGPlayerController::TogglePauseMenu()
{
	if (bMenuVisible)
	{
		UnpauseGame();
	}
	else
	{
		PauseGame();
	}
}

void ARPGPlayerController::Run()
{
}

void ARPGPlayerController::RunStop()
{
}

void ARPGPlayerController::Dodge()
{
}

void ARPGPlayerController::Select()
{
}
