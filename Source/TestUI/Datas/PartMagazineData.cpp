#include "PartMagazineData.h"
#include "GunStats.h"

UPartMagazineData::UPartMagazineData()
{
	PartSlotType = EPartSlot::Magazine;
}

void UPartMagazineData::ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const
{
	InOutStats.BaseDamage += DamageBonus;
}

