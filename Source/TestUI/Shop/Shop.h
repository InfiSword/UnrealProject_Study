// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shop.generated.h"

class UWrapBox;
class UScrollBox;
class UButton;
class AShopNPC;
class AMyTestCharacter;
class UShopSlot;
class UPartsData;

UCLASS()
class TESTUI_API UShop : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// [좌측: 상점 목록 (3x4 그리드 용도)]
	UPROPERTY(meta = (BindWidget))
	UWrapBox* ShopSlotGrid;

	// [우측: 내 인벤토리 목록 (1열 스크롤 용도)]
	UPROPERTY(meta = (BindWidget))
	UScrollBox* InventoryScrollBox;

	// [우측: 내 인벤토리 그리드 (3열 스크롤 용도)]
	UPROPERTY(meta = (BindWidget))
	UWrapBox* MyInventoryGrid;

	// [우측 하단: 구매 버튼]
	UPROPERTY(meta = (BindWidget))
	UButton* MainBuyButton;

	// [우측 하단: 판매 버튼] (UMG에 없으면 무시)
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* MainSellButton;

	// 테스트용 상점 데이터 재바인딩 버튼(없어도 동작)
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ShopRefreshButton;

	// 상점 닫기 버튼(UMG에 있으면 자동 바인딩)
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* MainExitButton;

	// 위젯 블루프린트 클래스 할당용
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<class UShopSlot> ShopSlotClass;

	// 인벤토리용 슬롯 위젯
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<class UInventorySlot> InvenSlotClass;

	// 상태 변수
	UPROPERTY()
	AShopNPC* CurrentShop;

	int32 CurrentSelectedIndex = -1;
	int32 CurrentSelectedPrice = 0;
	int32 CurrentSelectedInventoryIndex = -1;
	bool bShopGridInitialized = false;

	UPROPERTY()
	TArray<TObjectPtr<UShopSlot>> CachedShopSlots;

	UPROPERTY()
	TArray<TObjectPtr<UInventorySlot>> CachedInventorySlots;

	// 메인 함수
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void OpenShop(AShopNPC* InShop);

	// 나가기 버튼/O키 재입력 등 모든 닫기 경로가 공통으로 호출한다.
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void CloseShop();
	
	// 상점 데이터 재바인딩
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void RebindShopData();

	// 인벤토리 그리드를 3열 형태로 갱신
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void RefreshInventory();

	UFUNCTION()
	void HandleInventoryUpdated();

	// 상점 슬롯이 눌렸을 때 호출됨
	void SetSelectedItem(int32 SelectedIndex, int32 ItemPrice);

	// 인벤토리 슬롯이 눌렸을 때 호출됨
	void SetSelectedInventoryItem(int32 InventoryIndex);

	// 현재 플레이어의 돈을 확인하여 구매 버튼 활성화 여부 결정
	void UpdateBuyButtonState() const;

	// 현재 인벤토리 선택 여부를 확인하여 판매 버튼 활성화 여부 결정
	void UpdateSellButtonState() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void BuildShopSlotsOnce();
	int32 ResolveSellPriceForPart(const UPartsData* PartData) const;

	UFUNCTION()
	void OnMainBuyButtonClicked();

	UFUNCTION()
	void OnMainSellButtonClicked();

	UFUNCTION()
	void OnShopRefreshClicked();

	UFUNCTION()
	void OnMainExitButtonClicked();

private:
	void ClearCurrentSelection();
	void UnbindInventoryUpdatedDelegate();
};
