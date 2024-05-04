// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "C:/Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "C:/Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "C:/Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "C:/Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "TopDownPawn.generated.h"

DECLARE_DELEGATE_ThreeParams(ShowCameraParam , FString , FString, FString)

UCLASS()
class TOPDOWNCHARACTER_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopDownPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "MyTopDownPawn")
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "MyTopDownPawn")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "MyTopDownPawn")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly,Category = "MyTopDownPawn")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly,Category = "MyTopDownPawn")
	UInputAction* MoveAction;  

	UPROPERTY(EditDefaultsOnly,Category = "MyTopDownPawn")
	UInputAction* ScrollAction;
	
	UPROPERTY(EditDefaultsOnly , Category = "MyTopDownPawn")
	UFloatingPawnMovement* FloatingMovement;

	UFUNCTION(BlueprintCallable, Category = "MyTopDownPawn")
	void Move(const FInputActionValue& ActionValue);
	
	UFUNCTION(BlueprintCallable, Category = "MyTopDownPawn")
	void ZoomInOut(const FInputActionValue& ActionValue);

	UFUNCTION(BLueprintimplementableEvent)
	void ShowNotification(const FString& Offst_Z , const FString& ArmLength, const FString& Angle);

	ShowCameraParam NotificationMessage;
};
