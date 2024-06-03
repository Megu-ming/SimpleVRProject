// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "VRHandSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEVRPROJECT_API UVRHandSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	bool bMirror = false;
};
