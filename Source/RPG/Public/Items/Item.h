// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/ItemDataInfo.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UWidgetComponent;
class UItemDataInfo;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

enum class EOldItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemInformation
{
	GENERATED_BODY()
	
	FItemInformation() {}
	FItemInformation(int32 ID, FString Name, int32 Value, int32 Rarity, int32 ItemCount, UTexture2D* Thumbnail, USoundBase* PickupSound, USkeletalMesh* Mesh, const FGameplayTag& ItemTag, UGameplayEffect* ItemEffect)
	{
		this->ID = ID;
		this->Name = Name;
		this->Value = Value;
		this->Rarity = Rarity;
		this->ItemCount = ItemCount;
		this->Thumbnail = Thumbnail;
		this->PickupSound = PickupSound;
		this->Mesh = Mesh;
		this->ItemTag = ItemTag;
		this->ItemEffect = ItemEffect;
	}

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties")
	UGameplayEffect* ItemEffect;
	
};

UCLASS()
class RPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	TObjectPtr<UItemDataInfo> ItemDataInfo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Item Properties")
	FItemInformation ItemInfoInstance;

	UFUNCTION(BlueprintPure)
	float TransformSin();

	UFUNCTION(BlueprintPure)
	float TransformCos();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
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

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EOldItemState ItemState = EOldItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sine Parameters", meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;
};
