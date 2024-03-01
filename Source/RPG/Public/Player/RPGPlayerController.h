// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "RPGPlayerController.generated.h"

class AMagicCircle;
class ARPGCharacter;
class UCustomMovementComponent;
struct FInventoryItem;
class UWidgetComponent;
class ARPGWeapon;
class UDamageTextComponent;
class URPGInputConfig;
class URPGUserWidget;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class URPGAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARPGPlayerController();

	UFUNCTION(Blueprintable)
	FHitResult LineTraceForSpells(FHitResult &OutHitResult);

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlocked, bool bCriticalHit);

	UInputMappingContext* GetRPGContext() const { return RPGContext; }
	UInputMappingContext* GetClimbContext() const { return ClimbMappingContext; }

	void AddInputMappingContext(UInputMappingContext* ContextToAdd, int32 InPriority);
	void RemoveInputMappingContext(UInputMappingContext* ContextToRemove);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTabDown = false;

	virtual void Tick(float DeltaSeconds) override;
		
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void OnPlayEnterClimbState();
	void OnPlayExitClimbState();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCharacterAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCharacterJumping = false;

	UPROPERTY()
	APlayerController* PlayerController;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPauseGame();

	UFUNCTION(BlueprintImplementableEvent)
	void OnResumeGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* AttributeMenu = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WidgetClass;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnpauseGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* AttributePauseMenu = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> AttributePauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* SpellPauseMenu = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> SpellPauseMenuClass;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> RPGContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> ClimbMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ClimbMoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ItemPickupAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ToggleWalkAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ClimbAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ClimbHopAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ComboAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> TargetLockAction;

	// For the Pause Menu

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bReadingBook = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bPauseReadingBook = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input", Meta = (AllowPrivateAccess = true))
	bool bIsTargeting = false;
	
	bool bMenuVisible;
	
	void TogglePauseMenu();
	//void MenuSettings(bool bTabDown, bool bMenuVisible, bool bShowMouseCursor, FInputModeGameAndUI InputModeData);
	
	//bool bIsInAir;
	
	// Input Actions Callbacks
	UFUNCTION(BlueprintCallable)
	void Jump();
	
	void HandleGroundMovementInput(const FInputActionValue& Value);
	void HandleClimbMovementInput(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	void Select();
	void TabDown();
	void TabUp();
	void Run();
	void Dodge();
	void RunStop();
	
	void OnClimbActionStarted(const FInputActionValue& Value);

	void OnClimbHopActionStarted(const FInputActionValue& Value);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<URPGInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<URPGAbilitySystemComponent> RPGAbilitySystemComponent;

	URPGAbilitySystemComponent* GetASC();
	
	void LineTraceFromScreenCenter(FHitResult &HitResult, ECollisionChannel TraceChannel);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	//UPROPERTY()
	//UCustomMovementComponent* CustomMovementComponent;
	
	UPROPERTY()
	ARPGCharacter* RPGCharacter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
	
	void UpdateMagicCircleLocation();

	FHitResult LineTraceResult;
};
