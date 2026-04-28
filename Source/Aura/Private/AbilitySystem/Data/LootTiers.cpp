#include "AbilitySystem/Data/LootTiers.h"

#include "Kismet/GameplayStatics.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> Items;

	for (FLootItem& Loot : LootItems)
	{
		for (int32 i = 0; i < Loot.MaxNumToSpawn; i++)
		{
			int32 Chance = FMath::RandRange(1, 100);
			if (Chance < Loot.ChanceToSpawn)
			{
				FLootItem LootItem;
				LootItem.LootClass = Loot.LootClass;
				LootItem.bLootLevelOverride = Loot.bLootLevelOverride;
				Items.Add(LootItem);
			}
		}
	}
	return Items;
}
