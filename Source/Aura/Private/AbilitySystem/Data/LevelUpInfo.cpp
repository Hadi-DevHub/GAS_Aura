// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	int32 Level = 1;
	bool bSearching = true;

	while (bSearching)
	{
		// LevelUpInfos[1] = Level 1 Info
		// LevelUpInfos[2] = Level 2 Info
		if (LevelUpInfos.Num() - 1 <= Level) return Level;

		if (XP >= LevelUpInfos[Level].LevelUpRequirement)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
