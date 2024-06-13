// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/Projectile/Projectile.h"

AProjectile::AProjectile()
{
	SetLifeSpan(5.f);
}

void AProjectile::Init(UStaticMesh* const Mesh, const FTransform& InTransform, const FVector& InImpulse)
{
	if (!IsValid(Mesh)) { return; }

	ProceduralMeshComponent->SetSimulatePhysics(true);
	ProceduralMeshComponent->SetRelativeScale3D(InTransform.GetScale3D());
	ProceduralMeshComponent->SetRelativeRotation(InTransform.GetRotation());
	ProceduralMeshComponent->AddImpulse(InImpulse);
	StaticMeshComponent->SetStaticMesh(Mesh);
}

void AProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
