// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GrabComponent.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AActor* Owner = GetOwner();
	if (!Owner) { return; }
	Owner->GetComponents(UPrimitiveComponent::StaticClass(), OwnerPrimitiveComponents);

	OwnerPrimitiveInitTransforms.Reserve(OwnerPrimitiveComponents.Num());
	for (UPrimitiveComponent* It : OwnerPrimitiveComponents)
	{
		OwnerPrimitiveInitTransforms.Add(It->GetRelativeTransform());
	}
}

void UGrabComponent::Grab(UMotionControllerComponent* NewMotionControllerComponent)
{
	FDestructorBroadcastDelegate<FOnGrabbed> DestructorCallDelegate(OnGrabbed);

	GrabMotionController = NewMotionControllerComponent;

	AActor* Owner = GetOwner();
	Owner->SetActorEnableCollision(false);

	{
		APawn* PawnOwner = Cast<APawn>(GrabMotionController->GetOwner());
		Owner->SetOwner(PawnOwner);
		Owner->SetInstigator(PawnOwner);
	}
	for (UPrimitiveComponent* It : OwnerPrimitiveComponents)
	{
		It->SetSimulatePhysics(false);
		It->AttachToComponent(GrabMotionController, FAttachmentTransformRules::KeepWorldTransform);
	}

	if (bKeepWorld) { return; }

	const FTransform HandRotationTrnasform = FTransform(FRotator(-90., 0., 0.), FVector::ZeroVector, FVector::OneVector);
	const int32 Num = OwnerPrimitiveComponents.Num();

	const FTransform GrabComponentTransform = GetRelativeTransform();
	const FTransform GrabComponentRotationTransform = FTransform(GrabComponentTransform.GetRotation(),
		FVector::ZeroVector, FVector::OneVector);

	for (int32 i = 0; i < Num; ++i)
	{
		UPrimitiveComponent* It = OwnerPrimitiveComponents[i];
		const FTransform OwnerTransform = OwnerPrimitiveInitTransforms[i];

		// GrabComponent의 회전값이 보정된 Transform
		const FTransform GrabComponentRotationAdjustedTransform = UKismetMathLibrary::ComposeTransforms(OwnerTransform, GrabComponentRotationTransform);

		const FTransform GrabComponentRotationAndTranslationAdjustedTransform = FTransform(
			GrabComponentRotationAdjustedTransform.GetRotation(),
			GrabComponentRotationAdjustedTransform.GetLocation() - GrabComponentTransform.GetLocation(),
			GrabComponentRotationAdjustedTransform.GetScale3D());

		FTransform FinalTransform = UKismetMathLibrary::ComposeTransforms(GrabComponentRotationAndTranslationAdjustedTransform, HandRotationTrnasform);
		It->SetRelativeTransform(FinalTransform, false, nullptr, ETeleportType::ResetPhysics);
	}
}

void UGrabComponent::ReleaseGrab()
{
	FDestructorBroadcastDelegate<FOnReleaseGrabbed> DestructorCallDelegate(OnReleaseGrabbed);

	AActor* Actor = GetOwner();
	Actor->SetActorEnableCollision(true);
	Actor->SetOwner(nullptr);
	Actor->SetInstigator(nullptr);

	for (UPrimitiveComponent* It : OwnerPrimitiveComponents)
	{
		It->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		It->SetSimulatePhysics(true);
	}
}

// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

