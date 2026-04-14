#include "GameMode/AuraGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "GameMode/AuraGameInstance.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
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

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Add(DefaultMapName, DefaultMap);
}
