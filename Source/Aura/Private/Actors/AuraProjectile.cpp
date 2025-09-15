// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AuraProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	bReplicates = true;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	LoopingSFXComponent = UGameplayStatics::SpawnSoundAttached(MidAirLoopingSFX, GetRootComponent());
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileImpactVFX, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileImpactSFX, GetActorLocation());
	LoopingSFXComponent->Stop();
	
	if (HasAuthority())
	{
		Destroy();
	}
	else
	{
		bIsHit = true;
	}
}

void AAuraProjectile::Destroyed()
{
	if (!bIsHit && !HasAuthority())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileImpactVFX, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileImpactSFX, GetActorLocation());
		LoopingSFXComponent->Stop();
	}
	Super::Destroyed();
}

