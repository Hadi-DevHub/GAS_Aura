#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "GameMode/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2= NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(2, LoadSlot_2);
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
		AuraGameMode->SaveSlotData(LoadSlots[LoadSlotIndex], LoadSlotIndex);
		LoadSlots[LoadSlotIndex]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 LoadSlotIndex)
{
}

void UMVVM_LoadScreen::SetNumSlots(int32 InNumSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumSlots, InNumSlots);
}

