#include "PartScopeData.h"
#include "GunStats.h"

UPartScopeData::UPartScopeData()
{
	PartSlotType = EPartSlot::Scope;
}

void UPartScopeData::ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const
{
	InOutStats.FireRate = FMath::Max(0.01f, InOutStats.FireRate + FireRateDelta);
}

