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
	float ProjectileSpeed = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Projectile|Bullet")
	float ProjectileGravityScale = 0.f;

	UPROPERTY(EditAnywhere, Category = "Projectile\Eject")
	UStaticMesh* EjectStaticMesh;
	UPROPERTY(EditAnywhere, Category = "Projectile\Eject")
	FTransform EjectStaticMeshTransform;

	UPROPERTY(EditAnywhere, Category = "Projectile\Eject")
	bool bPhysics = false;
};

UCLASS()
class SIMPLEVRPROJECT_API AProjectile : public AProceduralMesh
{
	GENERATED_BODY()

public:
	AProjectile();

	void Init(UStaticMesh* const Mesh, const FTransform& InTransform, const FVector& InImpulse);

	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

};
