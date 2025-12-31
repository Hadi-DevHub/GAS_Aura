#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& DataUnderMouse);
	
	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> StoredHitActor;

	UPROPERTY(BlueprintReadOnly)
	FVector StoredHitLocation;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerController> StoredOwnerPlayerController;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> StoredOwnerAvatar;

	UPROPERTY(EditDefaultsOnly, Category = "BeamSpell")
	int32 MaxNumOfStoredTargets;

private:
};
