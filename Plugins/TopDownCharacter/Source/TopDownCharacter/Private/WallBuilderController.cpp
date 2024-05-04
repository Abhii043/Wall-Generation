// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBuilderController.h"

AWallBuilderController::AWallBuilderController()
{
	bShowMouseCursor = true;
	Message.BindUFunction(this, "NotifyMessage");
}

void AWallBuilderController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent); 

	MappingContext = NewObject<UInputMappingContext>(this); 

	OnClick = NewObject<UInputAction>(this);
	OnClick->ValueType = EInputActionValueType::Boolean;

	CreateNewSpline = NewObject<UInputAction>(this);
	CreateNewSpline->ValueType = EInputActionValueType::Boolean;

	NextSpline = NewObject<UInputAction>(this);
	NextSpline->ValueType = EInputActionValueType::Boolean;

	LastSpline = NewObject<UInputAction>(this);
	LastSpline->ValueType = EInputActionValueType::Boolean;

	LatestSpline = NewObject<UInputAction>(this);
	LatestSpline->ValueType = EInputActionValueType::Boolean;
	
	Undo = NewObject<UInputAction>(this);
	Undo->ValueType = EInputActionValueType::Boolean;

	DestroyWall = NewObject<UInputAction>(this);
	DestroyWall->ValueType = EInputActionValueType::Boolean;

	check(EIC)
		EIC->BindAction(OnClick , ETriggerEvent::Completed , this , &AWallBuilderController::GenerateWall);
		EIC->BindAction(CreateNewSpline, ETriggerEvent::Completed , this , &AWallBuilderController::NewSpline);
		EIC->BindAction(NextSpline, ETriggerEvent::Completed , this , &AWallBuilderController::ForwardSpline);
		EIC->BindAction(LastSpline, ETriggerEvent::Completed , this , &AWallBuilderController::BackWardSpline);
		EIC->BindAction(LatestSpline, ETriggerEvent::Completed , this , &AWallBuilderController::NewestSpline);
		EIC->BindAction(Undo, ETriggerEvent::Completed , this , &AWallBuilderController::UndoWall);
		EIC->BindAction(DestroyWall, ETriggerEvent::Completed , this , &AWallBuilderController::Destroy);

	if (MappingContext) {
		MappingContext->MapKey(OnClick, EKeys::LeftMouseButton);
		MappingContext->MapKey(CreateNewSpline, EKeys::RightMouseButton);
		MappingContext->MapKey(NextSpline, EKeys::Right);
		MappingContext->MapKey(LastSpline, EKeys::Left);
		MappingContext->MapKey(LatestSpline, EKeys::Up);
		MappingContext->MapKey(Undo, EKeys::Z);
		MappingContext->MapKey(DestroyWall, EKeys::B);
	}

	check(GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(SubSystem);
	SubSystem->AddMappingContext(MappingContext, 0);

}

void AWallBuilderController::GenerateWall()
{
	if(ArrayOfSplines.Num() != 0){
		FHitResult HitonClick; 
		GetHitResultUnderCursor(ECC_Visibility, true, HitonClick);

		if (HitonClick.bBlockingHit)
		{
			FVector ClickLocation = HitonClick.Location;
			ArrayOfSplines[SplineIndex]->Points.Add(HitonClick.Location);
			ArrayOfSplines[SplineIndex]->AddPoint(ClickLocation);

			if (ArrayOfSplines[SplineIndex]->Points.Num() >= 2) {
				FString Msg = "On Spline " + FString::FromInt(SplineIndex + 1) + " Wall " + FString::FromInt(ArrayOfSplines[SplineIndex]->Points.Num() - 1) + " Generated";
				Message.ExecuteIfBound(Msg);
			}
		}
	}
	else {
		FString Msg = "Right Click To Generate Spline";
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::NewSpline()
{
	if(ArrayOfSplines.Num() > 0){
		if (ArrayOfSplines[ArrayOfSplines.Num() - 1]->Points.Num() >= 2) {
			ASplineActor* Spline = NewObject<ASplineActor>(this);
			ArrayOfSplines.Add(Spline);
			SplineIndex = ArrayOfSplines.Num() - 1;

			FString Msg = "New Spline " + FString::FromInt(SplineIndex + 1) + " Generated";
			Message.ExecuteIfBound(Msg);
		}
		else {
			FString Msg = "Atleast Create A Wall Before Creating New Spline";
			Message.ExecuteIfBound(Msg);
		}
	}
	else {
		ASplineActor* Spline = NewObject<ASplineActor>(this);
		ArrayOfSplines.Add(Spline);
		SplineIndex = ArrayOfSplines.Num() - 1;

		FString Msg = "New Wall Spline " + FString::FromInt(SplineIndex + 1) + " Generated";
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::BackWardSpline()
{
	if(SplineIndex == 0){
		FString Msg = "Already On Last Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
	else {
		SplineIndex--;
		FString Msg = "Now Working On Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::ForwardSpline()
{
	if (SplineIndex == ArrayOfSplines.Num() - 1) {
		FString Msg = "Already On Latest Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
	else {
		SplineIndex++;
		FString Msg = "Now Working On Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
}   

void AWallBuilderController::NewestSpline()
{
	if (SplineIndex == ArrayOfSplines.Num() - 1) {
		FString Msg = "Already On Latest Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
	else {
		SplineIndex = ArrayOfSplines.Num() - 1;
		FString Msg = "Now Working On Latest Spline " + FString::FromInt(SplineIndex + 1);
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::UndoWall()
{
	ArrayOfSplines[SplineIndex]->Undo();
	if(ArrayOfSplines[SplineIndex]->Points.Num()>=1){
		FString Msg = "Wall " + FString::FromInt(ArrayOfSplines[SplineIndex]->Points.Num() - 1) + " Of Spline " + FString::FromInt(SplineIndex + 1) + " Is Destroyed";
		Message.ExecuteIfBound(Msg);
	}
	else {
		FString Msg = "There is No Wall To Destroy";
		Message.ExecuteIfBound(Msg);
	}
}

void AWallBuilderController::Destroy()
{
	ArrayOfSplines[SplineIndex]->DestroyAllWall();
	if (SplineIndex != (ArrayOfSplines.Num() - 1))
		ArrayOfSplines.RemoveAt(SplineIndex); 
	FString Msg = "Spline " + FString::FromInt(SplineIndex + 1) + " Is Destroyed";
	Message.ExecuteIfBound(Msg);
}
