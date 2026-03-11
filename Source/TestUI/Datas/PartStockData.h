#pragma once

#include "CoreMinimal.h"
#include "PartsData.h"
#include "PartStockData.generated.h"

struct FGunStats;

UCLASS()
class TESTUI_API UPartStockData : public UPartsData
{
	GENERATED_BODY()

public:
	UPartStockData();

	// 개머리판 장착 시 추가 데미지 보너스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Stats")
	float DamageBonus = 0.5f;

	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const override;
};

