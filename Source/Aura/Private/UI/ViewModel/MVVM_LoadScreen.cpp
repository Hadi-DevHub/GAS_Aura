#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "GameMode/AuraGameModeBase.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName("LoadSlot_0");
	LoadSlot_0->LoadSlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName("LoadSlot_1");
	LoadSlot_1->LoadSlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2= NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName("LoadSlot_2");
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlot_2->LoadSlotIndex = 2;
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetSlotForIndex(int Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 LoadSlotIndex)
{
	LoadSlots[LoadSlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 LoadSlotIndex, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode)
	{
		LoadSlots[LoadSlotIndex]->SetLoadSlotPlayerName(EnteredName);
		LoadSlots[LoadSlotIndex]->SetLoadSlotMapName(AuraGameMode->DefaultMapName);
		LoadSlots[LoadSlotIndex]->SlotStatus = ESaveSlotStatus::Taken;
		AuraGameMode->SaveSlotData(LoadSlots[LoadSlotIndex], LoadSlotIndex);
		LoadSlots[LoadSlotIndex]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 LoadSlotIndex)
{
	OnSelectButtonPressed.Broadcast();
	for (TTuple<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == LoadSlotIndex)
		{
			LoadSlot.Value->SelectButtonPressed.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->SelectButtonPressed.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[LoadSlotIndex];
}

void UMVVM_LoadScreen::DeleteSlotButtonPressed()
{
	if (SelectedSlot == nullptr) { return; }
	
	AAuraGameModeBase::DeleteSelectedSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->LoadSlotIndex);
	SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
	SelectedSlot->SelectButtonPressed.Broadcast(true);
	SelectedSlot->InitializeSlot();
	SelectedSlot = nullptr;
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	if (SelectedSlot == nullptr) { return; }
	
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode == nullptr) { return; }

	AuraGameMode->TravelToMap(SelectedSlot);
}

void UMVVM_LoadScreen::LoadSaveSlots()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (TTuple<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveGameObject = AuraGameModeBase->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		FString& PlayerName = SaveGameObject->PlayerName;
		LoadSlot.Value->SlotStatus = SaveGameObject->SlotStatus;
		LoadSlot.Value->SetLoadSlotMapName(SaveGameObject->MapName);
		LoadSlot.Value->SetLoadSlotPlayerName(PlayerName);
		LoadSlot.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::SetNumSlots(int32 InNumSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumSlots, InNumSlots);
}

