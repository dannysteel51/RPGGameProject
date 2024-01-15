// Copyright Dan Stull

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemDataInfo.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	int32 ID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	FString Name = "Default";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	int32 Value = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	int32 Rarity = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	int32 ItemCount = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	UTexture2D* Thumbnail = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	USoundBase* PickupSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	USkeletalMesh* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	TSubclassOf<UGameplayEffect>  ItemEffect = nullptr;
};

/**
 * 
 */
UCLASS()
class RPG_API UItemDataInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	TMap<FGameplayTag, UGameplayEffect*> ItemEffects;

	UFUNCTION(BlueprintCallable)
	UGameplayEffect* GetItemEffectClass(FGameplayTag ItemTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties")
	FItemData ItemData;
	
	UFUNCTION(Blueprintable, BlueprintPure)
	FString GetItemName() {return ItemData.Name;}

	UFUNCTION(Blueprintable, BlueprintPure)
	int32 GetItemID() const {return ItemData.ID; }

	UFUNCTION(Blueprintable, BlueprintPure)
	int32 GetItemValue() const {return ItemData.Value;}

	UFUNCTION(Blueprintable, BlueprintPure)
	int32 GetItemRarity() const {return ItemData.Rarity;}

	UFUNCTION(Blueprintable, BlueprintPure)
	int32 GetItemCount() const {return ItemData.ItemCount;}

	UFUNCTION(Blueprintable, BlueprintPure)
	UTexture2D* GetItemThumbnail() const {return ItemData.Thumbnail;}

	UFUNCTION(Blueprintable, BlueprintPure)
	USoundBase* GetItemPickupSound() const {return ItemData.PickupSound;}

	UFUNCTION(Blueprintable, BlueprintPure)
	USkeletalMesh* GetItemMesh() const {return ItemData.Mesh;}

	UFUNCTION(Blueprintable, BlueprintPure)
	FGameplayTag GetItemTag() const {return ItemData.ItemTag;}
	
	UFUNCTION(Blueprintable, BlueprintPure)
	TSubclassOf<UGameplayEffect> GetItemEffect() const {return ItemData.ItemEffect;}
};
