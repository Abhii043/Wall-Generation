// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPawn.h"
#include "C:/Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
ATopDownPawn::ATopDownPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));  
	SpringArm->TargetArmLength = -211.599960;
	SpringArm->TargetOffset = FVector(-520, 0, 540);
	RootComponent = SpringArm;
	
	float Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(Angle,0,0));
	Camera->SetupAttachment(SpringArm); 

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));

	NotificationMessage.BindUFunction(this , "ShowNotification");
}

// Called when the game starts or when spawned
void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);  
	APlayerController* Player = Cast<APlayerController>(GetController());

	MappingContext = NewObject<UInputMappingContext>();
	MoveAction = NewObject<UInputAction>();
	MoveAction->ValueType = EInputActionValueType::Axis2D; 

	UInputModifierNegate* Negation = NewObject<UInputModifierNegate>(this);

	FEnhancedActionKeyMapping& ForwardMovementmapping = MappingContext->MapKey(MoveAction, EKeys::W);  
	FEnhancedActionKeyMapping& BackwardMovementmapping = MappingContext->MapKey(MoveAction, EKeys::S);  
	BackwardMovementmapping.Modifiers.Add(Negation);

	UInputModifierSwizzleAxis* SwizzleRight = NewObject<UInputModifierSwizzleAxis>(this); 
	SwizzleRight->Order = EInputAxisSwizzle::YXZ;

	FEnhancedActionKeyMapping& RightMovementmapping = MappingContext->MapKey(MoveAction, EKeys::D);
	RightMovementmapping.Modifiers.Add(SwizzleRight);   

	FEnhancedActionKeyMapping& LeftMovementmapping = MappingContext->MapKey(MoveAction, EKeys::A); 
	LeftMovementmapping.Modifiers.Add(SwizzleRight);  
	LeftMovementmapping.Modifiers.Add(Negation);

	//For Zooming

	ScrollAction = NewObject<UInputAction>();
	ScrollAction->ValueType = EInputActionValueType::Axis1D;

	FEnhancedActionKeyMapping& Zoom = MappingContext->MapKey(ScrollAction , EKeys::MouseWheelAxis); 
	
	if (EIC) {
		EIC->BindAction(MoveAction , ETriggerEvent::Triggered ,this ,&ATopDownPawn::Move);
		EIC->BindAction(ScrollAction , ETriggerEvent::Triggered ,this ,&ATopDownPawn::ZoomInOut);
	}

	//Binding Actions
	if(Player){
		ULocalPlayer* LocalPlayer = Player->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		//SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(MappingContext, 0);
	}

}

void ATopDownPawn::Move(const FInputActionValue& ActionValue)
{
	FVector InputAxis = ActionValue.Get<FVector>();
	AddMovementInput(InputAxis , 2.0F);
}

void ATopDownPawn::ZoomInOut(const FInputActionValue& ActionValue)
{
	float Value = ActionValue.Get<float>();

	float TargetArmLength = SpringArm->TargetArmLength;
	float TargetOffsetZ = SpringArm->TargetOffset.Z;

	if (TargetArmLength >= -460) {
		SpringArm->TargetOffset.Z -= Value * 20;
	}
	if (TargetOffsetZ <= 1000) {
		SpringArm->TargetArmLength -= Value * 9.2;
	}

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, -500, -0);
	SpringArm->TargetOffset.Z = FMath::Clamp(SpringArm->TargetOffset.Z, 0, 3000);

	float Angle;
	if (SpringArm->TargetArmLength == 0) {
		Angle = -90;
	}
	else {
		Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));
		Angle = FMath::Clamp(Angle, -90, 0);
	}

	if (Angle < -90) {
		Angle = -90;
	}
	if (Angle > 0) {
		Angle = 0;
	}

	FString Offset_Z = "Offset_Z: " + FString::SanitizeFloat(SpringArm->TargetOffset.Z);
	FString ArmLength ="ArmLength: " + FString::SanitizeFloat(SpringArm->TargetArmLength);
	FString Angle_ = "Angle: " + FString::SanitizeFloat(Angle);
	NotificationMessage.ExecuteIfBound(Offset_Z, ArmLength, Angle_); 

	Camera->SetRelativeRotation(FRotator(Angle, 0, 0));

	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("ROTATION: %f"), *Camera->GetRelativeRotation().ToString()));*/

}

