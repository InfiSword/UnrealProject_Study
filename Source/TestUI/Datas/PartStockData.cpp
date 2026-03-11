#include "PartStockData.h"
#include "GunStats.h"

UPartStockData::UPartStockData()
{
	PartSlotType = EPartSlot::Stock;
}

void UPartStockData::ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const
{
	InOutStats.BaseDamage += DamageBonus;
}

