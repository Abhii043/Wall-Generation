// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SplineActor.h"
#include "Delegates/Delegate.h" 
#include "WallBuilderController.generated.h"

DECLARE_DELEGATE_OneParam(GenerateMsg , FString)
/**
 * 
 */
UCLASS()
class TOPDOWNCHARACTER_API AWallBuilderController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWallBuilderController();

	virtual void SetupInputComponent() override; 

	UPROPERTY(EditDefaultsOnly , Category = "WallController")
	UInputAction* OnClick;
	
	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* Undo;
	
	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* DestroyWall;
	
	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* CreateNewSpline;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* NextSpline;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* LastSpline;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputAction* LatestSpline;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	TArray<ASplineActor*> ArrayOfSplines;

	UPROPERTY(EditDefaultsOnly, Category = "WallController")
	int32 SplineIndex;

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void GenerateWall();
	
	UFUNCTION(BlueprintCallable, Category = "WallController")
	void NewSpline();	

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void BackWardSpline();	

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void ForwardSpline();
	
	UFUNCTION(BlueprintCallable, Category = "WallController")
	void NewestSpline();

	UFUNCTION(BlueprintCallable, Category = "WallController")
	void UndoWall();
	
	UFUNCTION(BlueprintCallable, Category = "WallController")
	void Destroy();

	//Delegate And its Function

	GenerateMsg Message;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyMessage(const FString& msg);
};
