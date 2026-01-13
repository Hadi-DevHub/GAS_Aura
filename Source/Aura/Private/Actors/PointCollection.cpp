#include "Actors/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>(TEXT("RootPoint"));
	SetRootComponent(Pt_0);
	ImmutablePoints.AddUnique(Pt_0);
	
	Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_1"));
	Pt_1->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_1);
	
	Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_2"));
	Pt_2->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_2);
	
	Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_3"));
	Pt_3->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_3);
	
	Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_4"));
	Pt_4->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_4);
	
	Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_5"));
	Pt_5->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_5);
	
	Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_6"));
	Pt_6->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_6);
	
	Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_7"));
	Pt_7->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_7);
	
	Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_8"));
	Pt_8->SetupAttachment(GetRootComponent());
	ImmutablePoints.AddUnique(Pt_8);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride) const
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("Attempted to access ImmutablePoints out of bounds."))

	TArray<USceneComponent*> ArrayCopy;
	
	for (USceneComponent* Pt : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(ToPoint + Pt_0->GetComponentLocation());
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		FHitResult HitResult;
		TArray<AActor*> IgnoredActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetWorld(), IgnoredActors, TArray<AActor*>(), 1500.f, GetActorLocation());
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoredActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.Location.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		
		ArrayCopy.Add(Pt);
		
	}
	return ArrayCopy;
}
