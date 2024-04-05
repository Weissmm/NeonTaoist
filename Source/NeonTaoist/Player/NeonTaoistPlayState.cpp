// Fill out your copyright notice in the Description page of Project Settings.


#include "NeonTaoistPlayState.h"

void ANeonTaoistPlayerState::SellEquipment(UNeonTaoistEquipment* CurEquipment)
{
	if (Equipments.Contains(CurEquipment))
	{
		Equipments.Remove(CurEquipment);
		Market.Add(CurEquipment);
		CurEquipment->bIsEquipping = false;
	}
}

void ANeonTaoistPlayerState::BuyEquipment(UNeonTaoistEquipment* CurEquipment)
{
	if (Market.Contains(CurEquipment))
	{
		Market.Remove(CurEquipment);
		Equipments.Add(CurEquipment);
		CurEquipment->bIsEquipping = false;
	}
}

void ANeonTaoistPlayerState::EquipEquipment(UNeonTaoistEquipment* CurEquipment)
{
	if (Equipments.Contains(CurEquipment))
	{
		CurEquipment->bIsEquipping = true;
	}
}

void ANeonTaoistPlayerState::UnEquipEquipment(UNeonTaoistEquipment* CurEquipment)
{
	if (Equipments.Contains(CurEquipment))
	{
		CurEquipment->bIsEquipping = false;
	}
}