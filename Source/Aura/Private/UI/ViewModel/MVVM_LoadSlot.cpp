#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(const FString& InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetLoadSlotPlayerName(const FString& InLoadSlotPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotPlayerName, InLoadSlotPlayerName);
}

void UMVVM_LoadSlot::SetLoadSlotMapName(const FString& InLoadSlotMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotMapName, InLoadSlotMapName);
}

void UMVVM_LoadSlot::SetLoadPlayerLevel(const int32& InLoadPlayerLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadPlayerLevel, InLoadPlayerLevel);
}

