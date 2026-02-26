#pragma once

#include "CoreMinimal.h"
#include "GunStats.generated.h"

USTRUCT(BlueprintType)
struct FGunStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate = 0.1f;
};
