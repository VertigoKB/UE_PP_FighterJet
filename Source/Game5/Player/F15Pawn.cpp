// Fill out your copyright notice in the Description page of Project Settings.


#include "F15Pawn.h"
//Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

#include "Curves/CurveFloat.h"

//Components
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FighterRotateComponent.h"
//Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//EnhancedInput
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"

//Custom
#include "../CommonData/InputDataAsset.h"
#include "../CommonData/CommonEnum.h"
#include "../Interfaces/MissilesNumbering.h"
#include "../ObjectPool/ObjectPoolSystem.h"
#include "../ObjectPool/PoolingObject.h"
#include "../GameInstance/FighterGameInst.h"

// Sets default values
AF15Pawn::AF15Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	SetRootComponent(BoxRoot);
	BoxRoot->SetBoxExtent(FVector(950.f, 70.f, 60.f));
	BoxRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxRoot->SetCollisionObjectType(ECC_Pawn);
	BoxRoot->SetCollisionResponseToAllChannels(ECR_Block);

	ModelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	ModelMesh->SetupAttachment(BoxRoot);
	ModelMesh->SetRelativeLocation(FVector(0.f, 0.f, -200.f));

	//InputMappingContext
	const ConstructorHelpers::FObjectFinder<UInputMappingContext> FindMapping(TEXT("/Game/Blueprints/Input/IMC_Game5InputMapping.IMC_Game5InputMapping"));
	if (FindMapping.Succeeded())
		AirCraftInputMapping = FindMapping.Object;
	const ConstructorHelpers::FObjectFinder<UInputDataAsset> FindInputAction(TEXT("/Game/Blueprints/Input/InputDataBind.InputDataBind"));
	if (FindInputAction.Succeeded())
		InputAction = FindInputAction.Object;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeLocation(FVector(-1600.f, 0.f, 570.f));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CockpitCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CockpitCamera"));
	CockpitCamera->SetupAttachment(BoxRoot);
	CockpitCamera->SetRelativeLocation(FVector(500.f, 0.f, 140.f));

	InputRotateComponent = CreateDefaultSubobject<UFighterRotateComponent>(TEXT("InputRotateComponent"));

	Tags.Add(FName("Player"));

	IsMissileEmpty.Init(true, MaxLoadableMissile);
	LoadedMissiles.Init(nullptr, MaxLoadableMissile);

	PlayerState = EPlayerState::None;		//Trigger/SecondCutSceneTrigger will set state as PlayerState::CutScene
}

// Called when the game starts or when spawned
void AF15Pawn::BeginPlay()
{
	Super::BeginPlay();

	ThrustSpeed = 0.f;
	CurrentSpeed = 0.f;

	if (CockpitCamera)
		CockpitCamera->Deactivate();


	GetWorldTimerManager().SetTimer(HudValueExecuter, FTimerDelegate::CreateLambda([&]() {
		float RawZ = GetActorLocation().Z;
		float MappedZ = RawZ / 304.8f;
		if (MappedZ < 0.f)
			MappedZ = 0.f;
		if (MappedZ > 9999.f)
			MappedZ = 9999.f;
			
		float PlayerPitch = GetActorRotation().Pitch;
		float PlayerRoll = GetActorRotation().Roll;
		float MappedThrust = (ThrustSpeed / MaxThrustSpeed) * 650.f;
		OnReceiveHudValue.ExecuteIfBound(MappedThrust, MappedZ, PlayerPitch, PlayerRoll);
		}), 0.02f, true);
	
}

// Called every frame
void AF15Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePosition(DeltaTime);
	RotateAnimation(DeltaTime);


}

// Called to bind functionality to input
void AF15Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Enhanced Input Need to modify Build.cs
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(AirCraftInputMapping, 0);
	UEnhancedInputComponent* AircraftInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (AircraftInput)
	{
		AircraftInput->BindAction(InputAction->Throttle, ETriggerEvent::Triggered, this, &AF15Pawn::ThrottleInput);
		AircraftInput->BindAction(InputAction->NoseYaw, ETriggerEvent::Triggered, this, &AF15Pawn::YawInput);
		AircraftInput->BindAction(InputAction->NoseYaw, ETriggerEvent::Completed, this, &AF15Pawn::YawRelease);
		AircraftInput->BindAction(InputAction->NosePitch, ETriggerEvent::Triggered, this, &AF15Pawn::PitchInput);
		AircraftInput->BindAction(InputAction->NosePitch, ETriggerEvent::Completed, this, &AF15Pawn::PitchRelease);
		AircraftInput->BindAction(InputAction->NoseRoll, ETriggerEvent::Triggered, this, &AF15Pawn::RollInput);
		AircraftInput->BindAction(InputAction->NoseRoll, ETriggerEvent::Completed, this, &AF15Pawn::RollRelease);
		AircraftInput->BindAction(InputAction->LaunchProjectile, ETriggerEvent::Started, this, &AF15Pawn::LaunchInput);
		AircraftInput->BindAction(InputAction->ChangeView, ETriggerEvent::Started, this, &AF15Pawn::ChangeViewInput);
	}
}
void AF15Pawn::ThrottleInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	float CalcPosition = (Input * (GetWorld()->GetDeltaSeconds()) * ThrustMultiplier) + ThrustSpeed;

	ThrustSpeed = FMath::Clamp(CalcPosition, 0.f, MaxThrustSpeed);
}
void AF15Pawn::YawInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	InputRotateComponent->YawValue = Input;
}
void AF15Pawn::YawRelease(const FInputActionValue& Value)
{
	InputRotateComponent->YawValue = 0.f;
}
void AF15Pawn::PitchInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	InputRotateComponent->PitchValue = Input;
}
void AF15Pawn::PitchRelease(const FInputActionValue& Value)
{
	InputRotateComponent->PitchValue = 0.f;
}
void AF15Pawn::RollInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();

	InputRotateComponent->RollValue = Input;
}
void AF15Pawn::RollRelease(const FInputActionValue& Value)
{
	InputRotateComponent->RollValue = 0.f;
}

void AF15Pawn::LaunchInput(const FInputActionValue& Value)
{
	bool bInput = Value.Get<bool>();

	MissileAction();
}

void AF15Pawn::ChangeViewInput(const FInputActionValue& Value)
{
	RequestActiveCamera();
	OnViewChange.ExecuteIfBound();
}

//Update PawnTransform
void AF15Pawn::UpdatePosition(float DeltaSeconds)
{
	if (!(PlayerState == EPlayerState::CutScene))
		ThrustParam = ThrustSpeed;

	if (ThrustSpeed > 0.f)
		ThrustSpeed -= 2.3f;

	if (ThrustSpeed < CurrentSpeed)
		CurrentSpeed = FMath::FInterpTo(CurrentSpeed, ThrustSpeed, DeltaSeconds, Drag);
	else
		CurrentSpeed = ThrustSpeed;

	FVector NewPosition = (CurrentSpeed * DeltaSeconds) * GetActorForwardVector();
	
	if (PlayerState == EPlayerState::None)
		AppliedGravity = UKismetMathLibrary::MapRangeClamped(CurrentSpeed, 0.f, MinThrustToNotFall, Gravity, 0.f);
	else
		AppliedGravity = 0.f;

	float FallingScale = NewPosition.Z - (AppliedGravity * DeltaSeconds * 5.5f);
	NewPosition = FVector(NewPosition.X, NewPosition.Y, FallingScale);

	AddActorWorldOffset(NewPosition,true, nullptr, ETeleportType::TeleportPhysics);
}


void AF15Pawn::MissileAction()
{
	int32 Index = -1;
	IsMissileEmpty.Find(false, Index);

	if (Index != -1 || Index != INDEX_NONE)
	{
		IMissilesNumbering* Interface = Cast<IMissilesNumbering>(LoadedMissiles[Index]);
		Interface->Execute_MissileLaunch(LoadedMissiles[Index], Index);
		IsMissileEmpty[Index] = true;
		LoadedMissiles[Index] = nullptr;
	}
}

void AF15Pawn::RotateAnimation(float DeltaSeconds)
{
	if (RudderScale > 0.f)
		RudderScale -= DeltaSeconds * CtrlSurfacesRecoveryFactor;
	if (FlapsScale > 0.f)
		FlapsScale -= DeltaSeconds * CtrlSurfacesRecoveryFactor;
	if (AileronScale > 0.f)
		AileronScale -= DeltaSeconds * CtrlSurfacesRecoveryFactor;

	if (RudderScale < 0.f)
		RudderScale += DeltaSeconds * CtrlSurfacesRecoveryFactor;
	if (FlapsScale < 0.f)
		FlapsScale += DeltaSeconds * CtrlSurfacesRecoveryFactor;
	if (AileronScale < 0.f)
		AileronScale += DeltaSeconds * CtrlSurfacesRecoveryFactor;
}


void AF15Pawn::RequestActiveCamera()
{
	TObjectPtr<APlayerController> CastedController = Cast<APlayerController>(GetController());
	if (Camera->IsActive())
	{
		Camera->Deactivate();
		CockpitCamera->Activate();
		CastedController->SetViewTarget(this);
	}
	else
	{
		Camera->Activate();
		CockpitCamera->Deactivate();
		CastedController->SetViewTarget(this);
	}
}

FTransform AF15Pawn::GetMissileSocketLoaction(const uint8 SocketNum)
{
	EMissileSocket Socket = static_cast<EMissileSocket>(SocketNum);

	switch (Socket)
	{
	case EMissileSocket::MissileLO :
		return ModelMesh->GetSocketTransform(FName("MissileLO"));
	case EMissileSocket::MissileRO :
		return ModelMesh->GetSocketTransform(FName("MissileRO"));
	case EMissileSocket::MissileLI :
		return ModelMesh->GetSocketTransform(FName("MissileLI"));
	case EMissileSocket::MissileRI :
		return ModelMesh->GetSocketTransform(FName("MissileRI"));
	}
	return FTransform();
}

void AF15Pawn::InitLoadMissile(UObjectPoolSystem* Pool)
{
	for (int8 i = 0; i < MaxLoadableMissile; i++)
	{
		MissileLoadProcess(Pool, i);
	}
}

void AF15Pawn::MissileLoadProcess(UObjectPoolSystem* Pool, uint8 Num)
{
	LoadedMissiles[Num] = Pool->GetObject(GetMissileSocketLoaction(Num));
	IsMissileEmpty[Num] = false;
	IMissilesNumbering* Interface = Cast<IMissilesNumbering>(LoadedMissiles[Num]);
	Interface->Execute_MissileNumber(LoadedMissiles[Num], Num);
}