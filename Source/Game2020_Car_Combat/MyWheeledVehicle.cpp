// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWheeledVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

static const FName NAME_SteerInput("Steer");
static const FName NAME_ThrottleInput("Throttle");
	
AMyWheeledVehicle::AMyWheeledVehicle(){
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	Vehicle4W->MaxEngineRPM = 8000.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(7730.0f, 400.0f);

	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontBias = 0.6f;

	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritPitch = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.0f;

}

void AMyWheeledVehicle::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

}

void AMyWheeledVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AMyWheeledVehicle::ApplyThrottle);
	PlayerInputComponent->BindAxis(NAME_SteerInput, this, &AMyWheeledVehicle::ApplySteering);
	
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyWheeledVehicle::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMyWheeledVehicle::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("Reset", IE_Pressed, this, &AMyWheeledVehicle::ResetVehicle);
}

void AMyWheeledVehicle::ApplyThrottle(float val)
{
	GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AMyWheeledVehicle::ApplySteering(float val)
{
	GetVehicleMovementComponent()->SetSteeringInput(val);
}

void AMyWheeledVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AMyWheeledVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AMyWheeledVehicle::ResetVehicle()
{
	float Xpos, Ypos;
	Xpos = GetActorLocation().X;
	Ypos = GetActorLocation().Y;
	SetActorLocation({ Xpos, Ypos, 30 }, false, NULL, ETeleportType::TeleportPhysics);
	SetActorRelativeRotation({ 0, 0, 0 }, false, NULL, ETeleportType::TeleportPhysics);
}
