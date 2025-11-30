// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUD.generated.h"

class UAuraWidgetController;
class UAuraSpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:

	template <typename T>
	T* CreateOrGetWidgetController(TObjectPtr<T>& WidgetController, TSubclassOf<UAuraWidgetController> WidgetControllerClass, const FWidgetControllerParams& WCParams);
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	UAuraSpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAuraSpellMenuWidgetController> SpellMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraSpellMenuWidgetController> SpellMenuWidgetControllerClass;
private:
	
};

template <typename T>
T* AAuraHUD::CreateOrGetWidgetController(TObjectPtr<T>& WidgetController,
	TSubclassOf<UAuraWidgetController> WidgetControllerClass, const FWidgetControllerParams& WCParams)
{
	{
		if (WidgetController == nullptr)
		{
			WidgetController = NewObject<T>(this, WidgetControllerClass);
			WidgetController->SetWidgetControllerParams(WCParams);
			WidgetController->BindCallbacksToDependencies();
		}
		return WidgetController;
	}
}