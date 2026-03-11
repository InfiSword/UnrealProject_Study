#pragma once

#include "CoreMinimal.h"
#include "PartsData.h"
#include "PartGripData.generated.h"

struct FGunStats;

UCLASS()
class TESTUI_API UPartGripData : public UPartsData
{
	GENERATED_BODY()

public:
	UPartGripData();

	// 손잡이 장착 시 발사 간격 보정(음수면 연사 속도 증가)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Part Stats")
	float FireRateDelta = -0.005f;

	virtual void ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const override;
};

