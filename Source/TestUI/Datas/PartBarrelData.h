#pragma once

#include "CoreMinimal.h"
#include "PartsData.h"
#include "PartBarrelData.generated.h"

struct FGunStats;

UCLASS()
class TESTUI_API UPartBarrelData : public UPartsData
{
	GENERATED_BODY()

public:
	UPartBarrelData();

	// 배럴 장착 시 적용되는 고정 데미지 보너스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Stats")
	float DamageBonus = 2.0f;
	
	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const override;
};
