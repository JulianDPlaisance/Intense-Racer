// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn1.h"

const FName ACarPawn1::LookUpBinding("LookUp");
const FName ACarPawn1::LookRightBinding("LookRight");

ACarPawn1::ACarPawn1()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/game/vehicle/sedan/sedan_animBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

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

	ReverseSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm1"));
	ReverseSpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	ReverseSpringArm->SetRelativeRotation(FRotator(15.f, 0.f, 0.f));
	ReverseSpringArm->SetupAttachment(RootComponent);
	ReverseSpringArm->TargetArmLength = -900.0f;
	ReverseSpringArm->bEnableCameraRotationLag = true;
	ReverseSpringArm->CameraRotationLagSpeed = 7.f;
	ReverseSpringArm->bInheritPitch = false;
	ReverseSpringArm->bInheritRoll = false;

	ReverseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	ReverseCamera->SetupAttachment(ReverseSpringArm, USpringArmComponent::SocketName);
	ReverseCamera->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	ReverseCamera->bUsePawnControlRotation = true;
	ReverseCamera->FieldOfView = 90.f;



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
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ACarPawn1::OnHandbrakeReleased);	
	PlayerInputComponent->BindAction("LookBack", IE_Pressed, this, &ACarPawn1::SwitchCamera);
}

void ACarPawn1::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ACarPawn1::UpgradePhysicsMaterial()
{
}

void ACarPawn1::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ACarPawn1::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ACarPawn1::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ACarPawn1::Tick(float Delta)
{
	Super::Tick(Delta);

	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	//LookAtCar(ReverseCamera->GetOwner(), this->GetActorLocation());

	//UpdateHUDStrings();
}

void ACarPawn1::SwitchCamera()
{
	if (true == bLookBack)
	{

	}
	else
	{

	}
	bLookBack = !bLookBack;
}

void ACarPawn1::LookAtCar(AActor* LookingActor, FVector TargetPosition, FVector WorldUp)
{
	/*FVector Forward = TargetPosition - LookingActor->GetActorLocation();
	FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, WorldUp);
	LookingActor->SetActorRotation(Rot, ETeleportType::TeleportPhysics);
	/**/
}

void ACarPawn1::BeginPlay()
{
	Super::BeginPlay();
	bool bEnableInCar = false;
}
