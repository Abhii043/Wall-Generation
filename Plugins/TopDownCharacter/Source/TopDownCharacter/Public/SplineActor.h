// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>
#include "SplineActor.generated.h"

UCLASS()
class TOPDOWNCHARACTER_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly , Category = "MySpline")
	int32 SplinePoints;

	UPROPERTY(VisibleDefaultsOnly, Category = Defaults)
	USceneComponent* RootScene;

	UPROPERTY(VisibleDefaultsOnly , Category= Defaults)
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Defaults)
	UStaticMeshComponent* StaticMeshComponent;
	
	UFUNCTION(BlueprintCallable , Category = "MySpline")
	void GenerateSplineMeshWall() ;

	UPROPERTY(VisibleDefaultsOnly ,Category = "MySpline")
	TArray<USplineMeshComponent*> SplineComponentsArr;

	UPROPERTY(VisibleDefaultsOnly,Category = "MySpline")
	TArray<UStaticMeshComponent*> StaticMeshComponentsArr;

	UPROPERTY(EditAnyWhere , Category = "MySpline")
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnyWhere , Category = "MySpline")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnyWhere , Category = "MySpline")
	TEnumAsByte<ESplineMeshAxis::Type> FrowardAxis = ESplineMeshAxis::Type::X;

	UPROPERTY(VisibleDefaultsOnly ,Category = "MySpline")
	TArray<FVector> Points;

	UFUNCTION(BlueprintCallable, Category = "MySpline")
	void AddPoint(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "MySpline")
	void Undo();

	UFUNCTION(BlueprintCallable, Category = "MySpline")
	void DestroyAllWall();
};
