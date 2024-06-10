// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponActorComponent.generated.h"

USTRUCT()
struct FVRWeaponDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FVRWeaponDataTableRow();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FTransform MeshTransform;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FTransform GrabComponentTransform;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<UWeaponActorComponent> WeaponActorComponentClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEVRPROJECT_API UWeaponActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponActorComponent();

	virtual void SetData(const FVRWeaponDataTableRow* InDataTableRow);
	virtual void OnTriggerStarted();
	virtual void OnTriggerCompleted();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	class AWeapon* VRWeapon = nullptr;
	const FVRWeaponDataTableRow* DataTableRow;
};
