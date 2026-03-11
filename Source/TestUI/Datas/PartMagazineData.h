#pragma once

#include "CoreMinimal.h"
#include "PartsData.h"
#include "PartMagazineData.generated.h"

struct FGunStats;

UCLASS()
class TESTUI_API UPartMagazineData : public UPartsData
{
	GENERATED_BODY()

public:
	UPartMagazineData();

	// 탄창 장착 시 추가 데미지 보너스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Stats")
	float DamageBonus = 1.0f;

	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const override;
};

