#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectButtonPressed, bool, bEnable);


UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeSlot();

	UPROPERTY()
	int32 LoadSlotIndex = 0;
	
	UPROPERTY(BlueprintAssignable)
	FSelectButtonPressed SelectButtonPressed;

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void SetLoadSlotName(const FString& InLoadSlotName);
	void SetLoadSlotPlayerName(const FString& InLoadSlotPlayerName);
	void SetLoadSlotMapName(const FString& InLoadSlotMapName);

	FString GetLoadSlotName() const { return LoadSlotName; };
	FString GetLoadSlotPlayerName() const { return LoadSlotPlayerName; };;
	FString GetLoadSlotMapName() const { return LoadSlotMapName; }
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = ESaveSlotStatus::Vacant;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FString LoadSlotName = FString();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FString LoadSlotPlayerName = FString();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FString LoadSlotMapName = FString();
	
};
