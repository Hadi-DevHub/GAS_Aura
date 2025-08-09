// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacter.h"

AAuraCharacter::AAuraCharacter()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
