#include "GameMode/LoadScreenSaveGame.h"

FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName) const
{
	for (auto Map : SavedMaps)
	{
		if (Map.MapName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName) const
{
	for (auto Map : SavedMaps)
	{
		if (Map.MapName == InMapName)
		{
			return true;
		}
	}
	return false;
}
