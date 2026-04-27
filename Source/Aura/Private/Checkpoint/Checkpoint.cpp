#include "Checkpoint/Checkpoint.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameMode/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("Checkpoint Mesh");
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckpointMesh->SetupAttachment(GetRootComponent());
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->SetupAttachment(CheckpointMesh);

	UniqueMoveToLocation = CreateDefaultSubobject<UBoxComponent>("UniqueMoveToLocation");
	UniqueMoveToLocation->SetupAttachment(GetRootComponent());

	CheckpointMesh->SetCustomDepthStencilValue(StencilValueOverride);
	CheckpointMesh->MarkRenderStateDirty();
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (bBindToOverlapCallback)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
	}
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			UWorld* World = GetWorld();
			FString MapAssetName = World->GetMapName();
			MapAssetName.RemoveFromStart(World->StreamingLevelsPrefix);
			
			AuraGM->SaveWorldState(GetWorld(), MapAssetName);
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}

void ACheckpoint::HandleGlowEffects()
{
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterial);
	CheckpointReached(DynamicMaterial);
}

void ACheckpoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckpoint::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void ACheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void ACheckpoint::SetMoveToLocation_Implementation(FVector& OutLocation)
{
	OutLocation = UniqueMoveToLocation->GetComponentLocation();
}

