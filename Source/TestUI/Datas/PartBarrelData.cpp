// Fill out your copyright notice in the Description page of Project Settings.


#include "PartBarrelData.h"
#include "GunStats.h"

UPartBarrelData::UPartBarrelData()
{
	PartSlotType = EPartSlot::Barrel;
}

void UPartBarrelData::ApplyStatsToGun(FGunStats& InOutStats, const FPartInstance& InstanceData) const
{
	InOutStats.BaseDamage += DamageBonus;
}
