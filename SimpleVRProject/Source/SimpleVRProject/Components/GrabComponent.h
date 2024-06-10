// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrabComponent.generated.h"

template<class T>
struct FDestructorBroadcastDelegate
{
	FDestructorBroadcastDelegate(T& InDelegate)
		: Delegate(InDelegate) {}
	~FDestructorBroadcastDelegate() { Delegate.Broadcast(); }

	T& Delegate;
};

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnGrabbed, UGrabComponent, OnGrabbed);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnReleaseGrabbed, UGrabComponent, OnReleaseGrabbed);

class UMotionControllerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEVRPROJECT_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

	UFUNCTION(BlueprintCallable)
	void Grab(UMotionControllerComponent* NewMotionControllerComponent);
	UFUNCTION(BlueprintCallable)
	void ReleaseGrab();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnGrabbed OnGrabbed;
	UPROPERTY(BlueprintAssignable)
	FOnReleaseGrabbed OnReleaseGrabbed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bKeepWorld = false;
	UMotionControllerComponent* GrabMotionController;
	TArray<UPrimitiveComponent*> OwnerPrimitiveComponents;
	TArray<FTransform> OwnerPrimitiveInitTransforms;
};
