// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/VRHandSkeletalMeshComponent.h"
#include "Data/BasicInputDataConfig.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HandGraph.h"
#include "Components/GrabComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(GetRootComponent());

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));

	MotionControllerLeft->SetTrackingMotionSource(TEXT("LeftGrip"));
	MotionControllerRight->SetTrackingMotionSource(TEXT("RightGrip"));
	MotionControllerLeft->SetupAttachment(GetRootComponent());
	MotionControllerRight->SetupAttachment(GetRootComponent());

	GetMesh()->DestroyComponent();

	LeftHand = CreateDefaultSubobject<UVRHandSkeletalMeshComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(MotionControllerLeft);
	RightHand = CreateDefaultSubobject<UVRHandSkeletalMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(MotionControllerRight);

	HandGraphLeft = CreateDefaultSubobject<UHandGraph>(TEXT("HandGraphLeft"));
	HandGraphRight = CreateDefaultSubobject<UHandGraph>(TEXT("HandGraphRight"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> Class(TEXT("/Script/Engine.AnimBlueprint'/Game/StartMap/Hand/BPA_Hands.BPA_Hands_C'"));
	check(Class.Class);
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
		RightHand->SetSkeletalMeshAsset(Asset.Object);

		const FTransform Transform = FTransform(FRotator(25.0, 0.0, 90.0), FVector(-2.98, 3.5, 4.56));
		RightHand->SetRelativeTransform(Transform);
		RightHand->SetAnimClass(Class.Class);
	}
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
		LeftHand->SetSkeletalMeshAsset(Asset.Object);

		const FTransform Transform = FTransform(FRotator(-25.0, 180.0, 90.0), FVector(-2.98, -3.5, 4.56));
		LeftHand->SetRelativeTransform(Transform);
		LeftHand->bMirror = true;
		LeftHand->SetAnimClass(Class.Class);
	}
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
		Subsystem->AddMappingContext(BasicInputDataConfig->InputMappingContext, 0);

		const UVRHandsInputDataConfig* VRInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
		Subsystem->AddMappingContext(VRInputDataConfig->InputMappingContext, 0);

		const UVRHandsAnimationInputDataConfig* VRHandsAnimationInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();
		Subsystem->AddMappingContext(VRHandsAnimationInputDataConfig->InputMappingContext, 1);
	}
	else { check(false); }
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = MotionControllerLeft->GetComponentLocation();
	FVector ForwardVector = MotionControllerLeft->GetForwardVector();
	FVector RightVector = MotionControllerLeft->GetRightVector();
	FVector Cross = -ForwardVector.Cross(RightVector);
	FVector End = (Cross * TraceDist) + Start;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ensure(EnhancedInputComponent);

	{
		const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();
		EnhancedInputComponent->BindAction(BasicInputDataConfig->Move, ETriggerEvent::Triggered, this, &AVRCharacter::OnMove);
	}
	{
		const UVRHandsInputDataConfig* VRHandsInputDataConfig = GetDefault<UVRHandsInputDataConfig>();
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Started, this, &ThisClass::OnGrabLeftStarted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Left, ETriggerEvent::Completed, this, &ThisClass::OnGrabLeftCompleted);

		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Started, this, &ThisClass::OnGrabRightStarted);
		EnhancedInputComponent->BindAction(VRHandsInputDataConfig->IA_Grab_Right, ETriggerEvent::Completed, this, &ThisClass::OnGrabRightCompleted);
	}
	{
		HandGraphLeft->SetupPlayerInputComponent(MotionControllerLeft, EnhancedInputComponent);
		HandGraphRight->SetupPlayerInputComponent(MotionControllerRight, EnhancedInputComponent);
	}
}

bool AVRCharacter::PerformLineTrace(FHitResult& OutResult, 
	UMotionControllerComponent* InMotionControllerComponent)
{
	UMotionControllerComponent* MotionController = InMotionControllerComponent;
	FVector Start = MotionController->GetComponentLocation();
	FVector ForwardVector = MotionController->GetForwardVector();
	FVector RightVector = MotionController->GetRightVector();
	FVector Cross = -ForwardVector.Cross(RightVector);
	FVector End = (Cross * TraceDist) + Start;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	bool result =  GetWorld()->LineTraceSingleByChannel(OutResult, Start, End,
		ECollisionChannel::ECC_PhysicsBody, TraceParams);

	// Debug
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 3.f, 0, 1.f);
	
	return result;
}

bool AVRCharacter::CanGrab(FHitResult& OutResult)
{
	AActor* HitActor = OutResult.HitObjectHandle.GetManagingActor();
	if (!HitActor) { return false; }

	UGrabComponent* GrabComponent = HitActor->GetComponentByClass<UGrabComponent>();
	if (!GrabComponent) { return false; }

	const float Distance = GetDistanceTo(HitActor);
	if (Distance <= GrabDist)
		return true;
	else
		return false;
}

void AVRCharacter::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	const FRotator CameraRotator = VRCamera->GetRelativeRotation();
	const FRotator CameraYaw = FRotator(0., CameraRotator.Yaw, 0.);

	if (!FMath::IsNearlyZero(ActionValue.Y))
	{
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CameraYaw);
		AddMovementInput(ForwardVector, ActionValue.Y);
	}
	if (!FMath::IsNearlyZero(ActionValue.X))
	{
		const FVector RightVector = UKismetMathLibrary::GetRightVector(CameraYaw);
		AddMovementInput(RightVector, ActionValue.X);
	}
}

void AVRCharacter::OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue)
{
	FHitResult LineTraceResult;
	bool bIsLineTraceSuccess = PerformLineTrace(LineTraceResult, MotionControllerComponent);

	const FVector WorldLocation = MotionControllerComponent->GetComponentLocation();
	const float Radius = 60.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{ UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody) };
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	bool bIsSphereTraceSuccess = UKismetSystemLibrary::SphereTraceMultiForObjects(this, WorldLocation, WorldLocation,
		Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
	if (bIsLineTraceSuccess && CanGrab(LineTraceResult))
	{
		AActor* HitActor = LineTraceResult.HitObjectHandle.GetManagingActor();
		if (!HitActor) { return; }

		UGrabComponent* GrabComponent = HitActor->GetComponentByClass<UGrabComponent>();
		if (!GrabComponent) { return; }

		GrabComponent->Grab(MotionControllerComponent);

		bLeft ? LeftHandAttachedGrabComponent = GrabComponent : RightHandAttachedGrabComponent = GrabComponent;
		
		return;
	}
	else if (bIsSphereTraceSuccess)
	{
		for (FHitResult& It : HitResults)
		{
			AActor* HitActor = It.HitObjectHandle.GetManagingActor();
			if (!HitActor) { continue; }

			UGrabComponent* GrabComponent = HitActor->GetComponentByClass<UGrabComponent>();
			if (!GrabComponent) { continue; }

			GrabComponent->Grab(MotionControllerComponent);

			bLeft ? LeftHandAttachedGrabComponent = GrabComponent : RightHandAttachedGrabComponent = GrabComponent;

			return;
		}
	}
}

void AVRCharacter::OnGrabCompleted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue)
{
	UGrabComponent** TargetGrabComponentPointer = bLeft ? &LeftHandAttachedGrabComponent : &RightHandAttachedGrabComponent;
	UGrabComponent* TargetGrabComponent = *TargetGrabComponentPointer;
	if (!IsValid(TargetGrabComponent)) { return; }

	TargetGrabComponent->ReleaseGrab();

	*TargetGrabComponentPointer = nullptr;
}

