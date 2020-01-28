// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn1.h"

const FName ACarPawn1::LookUpBinding("LookUp");
const FName ACarPawn1::LookRightBinding("LookRight");

ACarPawn1::ACarPawn1()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	getMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/game/vehicle/sedan/sedan_animBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	UWheeledVehicleMovementComponent4w* Vehicle4w = CastChecked<UWheeledVehicleMovementComponent4w>(GetVehicleMovement());

	check(Vehicle4w->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = URacingPrototypeWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = URacingPrototypeWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = URacingPrototypeWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = URacingPrototypeWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	UMaterialInterface* Material = TextMaterial.Object;

	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;
}

void ACarPawn1::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACarPawn1::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACarPawn1::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ACarPawn1::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ACarPawn1::OnHandbreakReleased);	
}

void ACarPawn1::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ACarPawn1::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ACarPawn1::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ACarPawn1::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ACarPawn1::Tick(float Delta)
{
	Super::Tick(Delta);

	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	UpdateHUDStrings();
}

void ACarPawn1::BeginPlay()
{
	Super::BeginPlay();
	bool bEnableInCar = false;
}
