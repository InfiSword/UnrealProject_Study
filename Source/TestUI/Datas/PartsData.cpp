// Fill out your copyright notice in the Description page of Project Settings.


#include "PartsData.h"
#include "GunStats.h"

FPrimaryAssetId UPartsData::GetPrimaryAssetId() const
{
    // The Asset Manager needs a unique ID for each asset. For DataAssets, the FName of the asset is usually sufficient.
    // The PrimaryAssetType is set from the native class name.
    return FPrimaryAssetId(GetClass()->GetFName(), GetFName());
}

void UPartsData::ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const
{
	// 기본 파츠 데이터는 스탯을 변경하지 않는다.
}
