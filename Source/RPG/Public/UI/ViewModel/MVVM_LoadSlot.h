// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class RPG_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	void InitializeSlot();

	UPROPERTY()
	FString LoadSlotName;
	
	UPROPERTY()
	int32 LoadSlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	// Field Notifie

	void SetPlayerName(FString InPlayerName);
	void SetMapName(FString InMapName);
	
	FString GetPlayerName() const { return PlayerName; };
	FString GetMapName() const { return MapName; };

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString MapName;
	
};
