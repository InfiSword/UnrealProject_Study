#include "PartGripData.h"
#include "GunStats.h"

UPartGripData::UPartGripData()
{
	PartSlotType = EPartSlot::Grip;
}

void UPartGripData::ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const
{
	InOutStats.FireRate = FMath::Max(0.01f, InOutStats.FireRate + FireRateDelta);
}

