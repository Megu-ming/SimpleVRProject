// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "MISC/ActorPool.h"
#include "BulletDispenser.generated.h"

USTRUCT()
struct FDispenserDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(Category = "Projectile", EditAnywhere, meta = (RowType = "/Script/SIMPLEVRPROJECT.ProjectileDataTableRow"))
	FDataTableRowHandle ProjectileDataTable;
	UPROPERTY(Category = "Projectile", EditAnywhere, meta = (RowType = "/Script/SIMPLEVRPROJECT.ProceduralMeshDataTableRow"))
	FDataTableRowHandle ProjectileMeshTypeDataTable;

	UPROPERTY(EditAnywhere, Category = "Dispenser")
	TSubclassOf<UAnimInstance> AnimInstance;
	UPROPERTY(EditAnywhere, Category = "Dispenser")
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, Category = "Dispenser")
	float DelayTime;
	UPROPERTY(EditAnywhere, Category = "Dispenser")
	USkeletalMesh* SkeletalMesh;
};

UCLASS()
class SIMPLEVRPROJECT_API ABulletDispenser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletDispenser();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	void SetData(const FDispenserDataTableRow* InDispenserDataTableRow);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEjectCartridge(const FTransform& InEjectTransform);
	void SpawnProjectile(const FTransform& InTransform);

	void PlayAnimation();

protected:
	UPROPERTY(Category = "Projectile", EditAnywhere, meta = (RowType = "/Script/SIMPLEVRPROJECT.DispenserDataTableRow"))
	FDataTableRowHandle DispenserDT;

	const FDispenserDataTableRow* DispenserDataTableRow = nullptr;

private:
	FTimerHandle AnimationTimerHandle;
	UPROPERTY(EditAnywhere)
	UAnimMontage* FireMontage = nullptr;
	UPROPERTY(EditAnywhere)
	float DelayTime = 5.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
public:
	FActorPool EjectCartridgePool;
	FActorPool ProjectilePool;
	//FActorPool EffectPool;
};
