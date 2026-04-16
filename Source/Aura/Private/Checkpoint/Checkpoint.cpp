#include "Checkpoint/Checkpoint.h"
#include "Components/SphereComponent.h"

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
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag(FName("Player"))) { return; }

	HandleGlowEffects();
}

void ACheckpoint::HandleGlowEffects()
{
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterial);
	CheckpointReached(DynamicMaterial);
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

