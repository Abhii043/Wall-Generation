// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>("RootScene");
	RootComponent = RootScene;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SplineComponent->SetupAttachment(RootScene);
	SplineComponent->ClearSplinePoints();
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootScene);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x400.Wall_400x400'"));

	if (MeshAsset.Succeeded())
	{
		SplineMesh = MeshAsset.Object;
	}
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineActor::GenerateSplineMeshWall()
{
	if (Points.Num() >= 2) {
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
		SplineMeshComponent->RegisterComponent();
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->SetStaticMesh(SplineMesh);

		FVector StartTangent = SplineComponent->GetTangentAtSplinePoint((Points.Num() - 2) ,ESplineCoordinateSpace::Local);
		
		FVector EndTangent = SplineComponent->GetTangentAtSplinePoint((Points.Num() - 1) ,ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(Points[Points.Num() - 2], StartTangent, Points[Points.Num() - 1], EndTangent);
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetForwardAxis(FrowardAxis);
		//SplineMeshComponent->SetRelativeLocation(GetActorLocation());  
		SplineComponentsArr.Add(SplineMeshComponent);
	} 
}

void ASplineActor::AddPoint(FVector Location)
{
	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::Local);
	GenerateSplineMeshWall(); 
}

void ASplineActor::Undo()
{
	SplinePoints = SplineComponent->GetNumberOfSplinePoints();
	if(SplinePoints > 1){
		if (SplineComponentsArr[SplinePoints - 2]) {
			SplineComponentsArr[SplinePoints - 2]->DestroyComponent();
			SplineComponentsArr[SplinePoints - 2] = nullptr;
		}
		if (SplinePoints == 2) {
			SplineComponentsArr.Empty();
		}
		Points.RemoveAt(SplinePoints - 1);
		SplineComponent->RemoveSplinePoint((SplinePoints - 1), ESplineCoordinateSpace::Local);
	}
	else if (SplinePoints == 1) {
		Points.Empty();
		SplineComponent->RemoveSplinePoint((SplinePoints - 1), ESplineCoordinateSpace::Local);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("There is Nothing to Undo"));
	}
}

void ASplineActor::DestroyAllWall()
{
	for (int i = 0; i < SplineComponentsArr.Num(); i++) {
		if (SplineComponentsArr[i]) { 
			SplineComponentsArr[i]->DestroyComponent(); 
			SplineComponentsArr[i] = nullptr; 
		}
	}
	SplineComponent->ClearSplinePoints(); 
	Points.Empty();
	SplineComponentsArr.Empty();
} 

