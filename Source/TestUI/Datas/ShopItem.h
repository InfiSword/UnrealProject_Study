#pragma once

#include "CoreMinimal.h"
#include "PartsData.h" // 우리가 만든 파츠 데이터
#include "ShopItem.generated.h"

USTRUCT(BlueprintType)
struct FShopItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPartsData* ItemData = nullptr;

	// 상점에서 구매할 때 쓰는 가격(0이면 레거시/파츠 기본값으로 폴백)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Price", meta = (ClampMin = "0", UIMin = "0"))
	int32 BuyPrice = 0;

	// 판매 시 정산 가격(0이면 레거시/파츠 기본값/비율로 폴백)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Price", meta = (ClampMin = "0", UIMin = "0"))
	int32 SellPrice = 0;

	int32 GetBuyPrice() const
	{
		if (BuyPrice > 0)
		{
			return BuyPrice;
		}

		if (ItemData && ItemData->DefaultBuyPrice > 0)
		{
			return ItemData->DefaultBuyPrice;
		}

		return 1;
	}

	int32 GetSellPrice() const
	{
		if (SellPrice > 0)
		{
			return SellPrice;
		}

		if (ItemData && ItemData->DefaultSellPrice > 0)
		{
			return ItemData->DefaultSellPrice;
		}

		// 판매가를 별도 지정하지 않으면 구매가의 50% 기본값 적용
		return FMath::Max(1, GetBuyPrice() / 2);
	}
	
};