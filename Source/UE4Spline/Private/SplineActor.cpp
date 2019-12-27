// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Spline.h"
#include "SplineActor.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ASplineActor::ASplineActor(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("Spline"));
	SplineComp->SetMobility(EComponentMobility::Static);
	SplineComp->SetClosedLoop(true);
	RootComponent = SplineComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Road(TEXT("StaticMesh'/Game/TrackGenerator/Props/RoadMesh.RoadMesh'"));
	if (Road.Succeeded())
	{
		RoadMesh = Road.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GuardRailLeft(TEXT("StaticMesh'/Game/TrackGenerator/Props/L_GuardRail.L_GuardRail'"));
	if (GuardRailLeft.Succeeded())
	{
		GuardRailLeftMesh = GuardRailLeft.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GuardRailRight(TEXT("StaticMesh'/Game/TrackGenerator/Props/R_GuardRail.R_GuardRail'"));
	if (GuardRailRight.Succeeded())
	{
		GuardRailRightMesh = GuardRailRight.Object;
	}
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateSplineMesh();
}

void ASplineActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CreateSplineMesh();
}

#if WITH_EDITOR
void ASplineActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ClearSplineMesh();
	CreateSplineMesh();
}

void ASplineActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		ClearSplineMesh();
		CreateSplineMesh();
	}
}
void ASplineActor::PostEditUndo()
{
	Super::PostEditUndo();

	ClearSplineMesh();
	CreateSplineMesh();
}
#endif

void ASplineActor::AddMesh(const int32 PointIndex, UStaticMesh* StaticMesh)
{
	if (nullptr != SplineComp)
	{
		if (nullptr != StaticMesh)
		{
			const auto World = GetWorld();
			if (nullptr != World)
			{
				const auto SplineMeshComp = NewObject<USplineMeshComponent>(this);
				if (nullptr != SplineMeshComp)
				{
					const auto NextPointIndex = (PointIndex + 1) % SplineComp->GetNumberOfSplinePoints();
					//!< Get spline points
					FVector StartLocation, StartTangent;
					SplineComp->GetLocalLocationAndTangentAtSplinePoint(PointIndex, StartLocation, StartTangent);
					FVector EndLocation, EndTangent;
					SplineComp->GetLocalLocationAndTangentAtSplinePoint(NextPointIndex, EndLocation, EndTangent);

					//!< Start(End) point of mesh
					SplineMeshComp->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

					//!< Set mesh
					SplineMeshComp->SetStaticMesh(StaticMesh);

					//!< Slope, width, thickness
					SplineMeshComp->SetStartRoll(0.0f);
					SplineMeshComp->SetEndRoll(0.0f);
					SplineMeshComp->SetStartScale(FVector2D(1.0f, 1.0f));
					SplineMeshComp->SetEndScale(FVector2D(1.0f, 1.0f));
					
					SplineMeshComp->bSmoothInterpRollScale = true;
					SplineMeshComp->SetMobility(EComponentMobility::Static);
					SplineMeshComp->RegisterComponent();

					//!< Collision
					SplineMeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
					//SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}
			}
		}
	}
}

void ASplineActor::CreateSplineMesh()
{
	if (nullptr != SplineComp)
	{
		const auto Num = SplineComp->GetNumberOfSplinePoints() - (SplineComp->IsClosedLoop() ? 0 : 1);
		for (auto i = 0; i < Num; ++i)
		{
			if (nullptr != RoadMesh)
			{
				AddMesh(i, RoadMesh);
			}
			if (nullptr != GuardRailLeftMesh)
			{
				AddMesh(i, GuardRailLeftMesh);
			}
			if (nullptr != GuardRailRightMesh)
			{
				AddMesh(i, GuardRailRightMesh);
			}
		}
	}
}

void ASplineActor::ClearSplineMesh()
{
	if (nullptr != SplineComp)
	{
		TArray<UActorComponent*> Comps;
		GetComponents(USplineMeshComponent::StaticClass(), Comps);
		for (auto i : Comps)
		{
			i->UnregisterComponent();
		}
	}
}
