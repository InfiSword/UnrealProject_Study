

#pragma once

#include "CoreMinimal.h"
#include "PartsData.h"
#include "PartBarrelData.generated.h"

UCLASS()
class TESTUI_API UPartBarrelData : public UPartsData
{
	GENERATED_BODY()

public:
	// 배럴만의 고유 변수 (1레벨당 오르는 데미지)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Stats")
	float DamageIncreasePerLevel = 2.0f;

	// 부모의 가상 함수 구현 (오버라이드)
	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const override
	{
		// 총의 데미지 = 기본 데미지 + (배럴 고유 데미지 * 내 아이템의 현재 레벨)
		InOutStats.BaseDamage += (DamageIncreasePerLevel * InstanceData.CurrentLevel);
	}
};
