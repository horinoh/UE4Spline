// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

UCLASS()
class UE4SPLINE_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//!< AActor
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
#endif

	UFUNCTION(BlueprintCallable, Category = StaticMesh)
	void AddMesh(const int32 PointIndex, UStaticMesh* StaticMesh);

	UFUNCTION(BlueprintCallable, Category = Spline)
	void CreateSplineMesh();
	UFUNCTION(BlueprintCallable, Category = Spline)
	void ClearSplineMesh();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline)
	class USplineComponent* SplineComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	class UStaticMesh* RoadMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	class UStaticMesh* GuardRailLeftMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	class UStaticMesh* GuardRailRightMesh;
};
