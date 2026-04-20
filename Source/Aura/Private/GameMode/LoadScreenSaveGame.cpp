#include "GameMode/LoadScreenSaveGame.h"

FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName) const
{
	const FSavedMap* FoundMap = SavedMaps.FindByPredicate([InMapName](const FSavedMap* InSavedMaps)
	{
		return InSavedMaps->MapName == InMapName;
	});

	return FoundMap ? *FoundMap : FSavedMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName) const
{
	return SavedMaps.ContainsByPredicate([InMapName](const FSavedMap& Map) {
	  return Map.MapName == InMapName;
   });
}
