// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	APawn* Pawn = Cast<APawn>(GetAvatarActor());
	if (Pawn)
	{
		AAuraPlayerController* PC = Cast<AAuraPlayerController>(Pawn->GetController());
		if (PC)
		{
			FHitResult UnderCursor;
			PC->GetHitResultUnderCursor(ECC_Visibility, false, UnderCursor);
			if (UnderCursor.bBlockingHit)
			{
				FVector UnderCursorLocation = UnderCursor.ImpactPoint;
				ValidData.Broadcast(UnderCursorLocation);
			}
		}
	}
}
