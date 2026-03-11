// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Datas/ShopItem.h"
#include "ShopSlot.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UShop;

UCLASS()
class TESTUI_API UShopSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget)) UImage* SlotBackground;    // 배경색 (투명 버튼 위에)
	UPROPERTY(meta = (BindWidget)) UImage* ItemIcon;          // 파츠 아이콘
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemName;      // 이름
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemPrice;     // 가격
	UPROPERTY(meta = (BindWidget)) UTextBlock* PartTypeText;  // 파츠 종류 (텍스트로 표시할 경우)
	UPROPERTY(meta = (BindWidget)) UButton* SlotSelectButton; // 선택 버튼 (투명, 최상단에 배치)

	UPROPERTY() UShop* MainWidget;

	FShopItem itemData;
	int32 ItemIndex;
	bool bIsSoldOut = false;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SetupSlot(const FShopItem& InItem, int32 InIndex, UShop* InParent);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SetSoldOut();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SetAvailable();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SetEmpty();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void SetSelected(bool bSelected);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSlotSelected();

private:
	bool bIsSelected = false;
	FLinearColor OriginalBackgroundColor;
};
