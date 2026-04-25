#include "GameMode/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/AuraLogChannel.h"
#include "GameFramework/PlayerStart.h"
#include "GameMode/AuraGameInstance.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/NameAsStringIndexProxyArchive.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetLoadSlotPlayerName();
	LoadScreenSaveGame->SavedPlayerLevel = LoadSlot->GetLoadPlayerLevel();
	LoadScreenSaveGame->MapName = LoadSlot->GetLoadSlotMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->SlotStatus = ESaveSlotStatus::Taken;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

void AAuraGameModeBase::DeleteSelectedSlot(const FString& LoadSlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& LoadSlotName, int32 SlotIndex)
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotName, SlotIndex))
	{
		 SaveGameObject = UGameplayStatics::LoadGameFromSlot(LoadSlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
	if (LoadSlot == nullptr) { return; }

	const FString LoadSlotName = LoadSlot->GetLoadSlotName();
	const int32 SlotIndex = LoadSlot->LoadSlotIndex;
	const FString MapName = LoadSlot->GetLoadSlotMapName();

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps.FindChecked(MapName));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	if (!IsValid(AuraGameInstance)) { return nullptr; }
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for(AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AAuraGameModeBase::SaveWorldState(UWorld* World)
{
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	
	ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	if (SaveGame)
	{
		if (!SaveGame->HasMap(MapName))
		{
			FSavedMap NewMap;
			NewMap.MapName = MapName;
			SaveGame->SavedMaps.Add(NewMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(MapName);
		SavedMap.SavedActors.Empty();

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			
			FSavedActor ActorToSave;
			ActorToSave.ActorName = Actor->GetFName();
			ActorToSave.ActorTransform = Actor->GetActorTransform();

			FMemoryWriter MemoryWriter(ActorToSave.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);

			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			SavedMap.SavedActors.Add(ActorToSave);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapName == MapName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World)
{
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());

	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		ULoadScreenSaveGame* LoadGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (LoadGame == nullptr)
		{
			UE_LOG(LogAura, Warning, TEXT("Failed to load slot!."))
			return;
		}

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SaveActor : LoadGame->GetSavedMapWithMapName(MapName).SavedActors)
			{
				if (SaveActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SaveActor.ActorTransform);
					}
					FMemoryReader MemoryReader(SaveActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);
						
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
 
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	if (!IsValid(AuraGameInstance)) { return nullptr; }

	FString LoadSlotName = AuraGameInstance->LoadSlotName;
	int32 LoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	
	return GetSaveSlotData(LoadSlotName, LoadSlotIndex);
}

void AAuraGameModeBase::SaveInGameProgress(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	FString SlotName = AuraGameInstance->LoadSlotName;
	int32 SlotIndex = AuraGameInstance->LoadSlotIndex;
	
	UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, SlotIndex);
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
