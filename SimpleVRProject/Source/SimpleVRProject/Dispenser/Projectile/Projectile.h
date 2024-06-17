// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obejct/ProceduralMesh.h"
#include "Components/ArrowComponent.h"
#include "Projectile.generated.h"

USTRUCT()
struct FProjectileDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Projectile\Bullet")
	UStaticMesh* BulletStaticMesh;
	UPROPERTY(EditAnywhere, Category = "Projectile\Bullet")
	FTransform BulletProceduralMeshTransform;

	UPROPERTY(EditAnywhere, Category = "Projectile|Bullet")
	float ProjectileSpeed = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Projectile|Bullet")
	UParticleSystem* GunShotEffect;

	UPROPERTY(EditAnywhere, Category = "Projectile\Eject")
	UStaticMesh* EjectStaticMesh;
	UPROPERTY(EditAnywhere, Category = "Projectile\Eject")
	FTransform EjectStaticMeshTransform;

};

UCLASS()
class SIMPLEVRPROJECT_API AProjectile : public AProceduralMesh
{
	GENERATED_BODY()

public:
	AProjectile();

	void Init(FProjectileDataTableRow* InDataTableRow, const FVector& InImpulse);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	virtual void SetData() override;

public:
	UFUNCTION()
	void OnHitAnyObject(UPrimitiveComponent* HitComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnParticleSpawned(FName EventName, float EmitterTime, FVector Location, FVector Velocity);

protected:
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* HitEffectParticleSystemComponent;
	UDataTable* DataTable;
};
