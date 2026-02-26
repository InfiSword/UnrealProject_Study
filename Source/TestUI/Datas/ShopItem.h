#pragma once

#include "CoreMinimal.h"
#include "PartsData.h" // 우리가 만든 파츠 데이터
#include "ShopItem.generated.h"

USTRUCT(BlueprintType)
struct FShopItem
{
	GENERATED_BODY()

	// 1. 무엇을 파는가? (원본 데이터 에셋)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPartsData* ItemData = nullptr;

	// 2. 얼마에 파는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 100;

	// (선택) 몇 레벨짜리를 파는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelToGive = 1;
};