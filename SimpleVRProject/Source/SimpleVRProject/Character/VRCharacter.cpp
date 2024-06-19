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
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(GetRootComponent());

	MotionControllerAimLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerAimLeft"));
	MotionControllerAimRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerAimRight"));

	WidgetInteractionLeft = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionLeft"));
	WidgetInteractionRight = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionRight"));

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));

	MotionControllerLeft->SetTrackingMotionSource(TEXT("LeftGrip"));
	MotionControllerRight->SetTrackingMotionSource(TEXT("RightGrip"));
	MotionControllerLeft->SetupAttachment(GetRootComponent());
	MotionControllerRight->SetupAttachment(GetRootComponent());
	{
		MotionControllerAimLeft->SetTrackingMotionSource(LeftAim);
		MotionControllerAimRight->SetTrackingMotionSource(RightAim);
		MotionControllerAimLeft->SetupAttachment(GetRootComponent());
		MotionControllerAimRight->SetupAttachment(GetRootComponent());
		WidgetInteractionLeft->SetupAttachment(MotionControllerAimLeft);
		WidgetInteractionLeft->PointerIndex = 0;
		WidgetInteractionLeft->InteractionDistance = 1000.f;
		WidgetInteractionLeft->bShowDebug = true;
		WidgetInteractionRight->SetupAttachment(MotionControllerAimLeft);
		WidgetInteractionRight->PointerIndex = 1;
		WidgetInteractionRight->InteractionDistance = 1000.f;
		WidgetInteractionRight->bShowDebug = true;
	}
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

	MenuWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MenuWidget"));
	MenuWidgetComponent->SetupAttachment(VRCamera);
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> Asset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/StartMap/UI/UI_Menu.UI_Menu_C'"));
		ensure(Asset.Class);
		MenuWidgetComponent->SetWidgetClass(Asset.Class);
		MenuWidgetComponent->SetRelativeLocation(FVector(400.f, 0.f, 0.f));
		MenuWidgetComponent->SetDrawSize(FVector2D(800.f, 800.f));
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

	//MenuWidgetComponent->GetWidget()->AddToPlayerScreen();
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		EnhancedInputComponent->BindAction(BasicInputDataConfig->Turn, ETriggerEvent::Triggered, this, &AVRCharacter::OnTurn);
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
	FVector End = (ForwardVector * TraceDist) + Start;

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
	AActor* HitActor = OutResult.HitObjectHandle.GetManagingActor();;
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

void AVRCharacter::OnTurn(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	
	AddControllerYawInput(ActionValue);
}

void AVRCharacter::OnGrabStarted(UMotionControllerComponent* MotionControllerComponent, const bool bLeft, const FInputActionValue& InputActionValue)
{
	FHitResult LineTraceResult;
	bool bIsLineTraceSuccess;
	if(bLeft)
		bIsLineTraceSuccess = PerformLineTrace(LineTraceResult, MotionControllerAimLeft);
	else
		bIsLineTraceSuccess = PerformLineTrace(LineTraceResult, MotionControllerAimRight);

	const FVector WorldLocation = MotionControllerComponent->GetComponentLocation();
	const float Radius = 50.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{ UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2) };
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	const bool bIsSphereTraceSuccess = UKismetSystemLibrary::SphereTraceMultiForObjects(this, WorldLocation, WorldLocation,
		Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
	AActor* DebugActor = LineTraceResult.GetActor();
	if(DebugActor)
		UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugActor->GetName());
	const bool bCanGrab = CanGrab(LineTraceResult);
	if (bIsLineTraceSuccess && bCanGrab)
	{
		AActor* HitActor = LineTraceResult.HitObjectHandle.GetManagingActor(); //LineTraceResult.HitObjectHandle.GetManagingActor();
		//if (!HitActor) { return; }

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

