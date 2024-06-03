// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VRHandAnimInstance.h"
#include "Components/VRHandSkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#if WITH_EDITOR
#include "Animation/DebugSkelMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#endif

void UVRHandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	USkeletalMeshComponent* Mesh = GetOwningComponent();
#if WITH_EDITOR
	UDebugSkelMeshComponent* DebugMesh = Cast<UDebugSkelMeshComponent>(Mesh);
	if (DebugMesh) { return; }

	AActor* Actor = GetOwningActor();
	ASkeletalMeshActor* SkelActor = Cast<ASkeletalMeshActor>(Actor);
	if (SkelActor) { return; }
#endif

	UVRHandSkeletalMeshComponent* VRMesh = Cast<UVRHandSkeletalMeshComponent>(Mesh);
	check(VRMesh);
	if(VRMesh)
		bMirror = VRMesh->bMirror;
}

void UVRHandAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CurrentPoseAlphaPoint = UKismetMathLibrary::FInterpTo(CurrentPoseAlphaPoint, PoseAlphaPoint, DeltaSeconds, 13.0);
	CurrentPoseThumb = UKismetMathLibrary::FInterpTo(CurrentPoseThumb, PoseAlphaThumb, DeltaSeconds, 13.0);
}