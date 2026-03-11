// Copyright (c) 2024, Your Name or Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Player/BaseCharacter.h"
#include "../Datas/ShopItem.h"
#include "IShopInterface.h"
#include "ShopNPC.generated.h"

class AMyTestCharacter;
class UShop;

UCLASS()
class TESTUI_API AShopNPC : public ABaseCharacter, public IShopInterface
{
	GENERATED_BODY()
	
public:	
	AShopNPC();

	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract(ABaseCharacter* Interactor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	TArray<FShopItem> ItemsForSale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Data", meta = (ClampMin = "1", UIMin = "1"))
	int32 ShopSlotCount = 12;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	TArray<bool> SoldOutStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop|Interaction", meta = (ClampMin = "10", UIMin = "10"))
	float InteractionDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shop|UI")
	TSubclassOf<UShop> ShopWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UShop> ActiveShopWidget;

public:	
	const TArray<FShopItem>& GetItemsForSale() const { return ItemsForSale; }

	virtual bool OpenShop_Implementation(AMyTestCharacter* Player) override;
	virtual void CloseShop_Implementation() override;
	virtual bool IsShopOpen_Implementation() const override { return bIsShopOpen; }

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void MarkItemSoldOut(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shop")
	bool IsItemSoldOut(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void RebuildShopDataForTest();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void ResetSoldOutStates();

private:
	void SyncSoldOutStateSize();
	void NormalizeItemPrices();
	bool BuildItemsForSaleFromDataManager();

	UPROPERTY()
	ABaseCharacter* CurrentInteractor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsShopOpen = false;
};
