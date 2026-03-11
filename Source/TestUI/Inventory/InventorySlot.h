// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Datas/PartsData.h"
#include "InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UInventoryMainWidget;
class UShop;

UCLASS()
class TESTUI_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UImage* SlotBackground;
	UPROPERTY(meta = (BindWidget)) UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PartTypeText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* EquipStatusText;
	UPROPERTY(meta = (BindWidget)) UButton* SlotSelectButton;

	UPROPERTY() UInventoryMainWidget* MainWidget;
	UPROPERTY() UShop* ShopMainWidget;

	FPartInstance ItemInstance;
	int32 ItemIndex = INDEX_NONE;
	bool bIsEquipped = false;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetupInvenSlot(const FPartInstance& InInstance, int32 InIndex, bool bEquipped = false, UShop* InShopMainWidget = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSelected(bool bSelected);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSlotSelected();

private:
	bool bIsSelected = false;
	FLinearColor OriginalBackgroundColor;
};
