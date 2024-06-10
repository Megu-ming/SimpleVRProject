// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/GrabComponent.h"
#include "Components/WeaponActorComponent.h"
#include "Weapon.generated.h"

UCLASS()
class SIMPLEVRPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void SetData(const FVRWeaponDataTableRow* VRWeaponDataTableRow);

	UAnimInstance* GetAnimInstance();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGrab();
	UFUNCTION()
	void OnReleaseGrab();

	UFUNCTION()
	void OnTriggerStarted();
	UFUNCTION()
	void OnTriggerCompleted();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/SIMPLEVRPROJECT.VRWeaponDataTableRow"))
	FDataTableRowHandle VRWeaponDT;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGrabComponent* GrabComponent;
	
protected:
	UPROPERTY(Transient)
	UWeaponActorComponent* WeaponActorComponent;
};
