// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/Projectile/Projectile.h"
#include "KismetProceduralMeshLibrary.h"
#include "Weapon/Weapon.h"

AProjectile::AProjectile()
{
	HitEffectParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffectParticleSystemComponent"));
	ProceduralMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
}

void AProjectile::Init(FProjectileDataTableRow* InDataTableRow, FProceduralMeshDataTableRow* InMeshDataTableRow, const FVector& InImpulse)
{
	if (!InDataTableRow) { ensure(false); return; }
	if (!IsValid(InDataTableRow->BulletStaticMesh)) { ensure(false); return; }
	if (!InMeshDataTableRow) { ensure(false); return; }

	MeshDataTableRow = InMeshDataTableRow;

	ProceduralMeshComponent->SetRelativeScale3D(InDataTableRow->BulletProceduralMeshTransform.GetScale3D());
	ProceduralMeshComponent->SetRelativeRotation(InDataTableRow->BulletProceduralMeshTransform.GetRotation());
	StaticMeshComponent->SetStaticMesh(InDataTableRow->BulletStaticMesh);

	UKismetProceduralMeshLibrary::
		CopyProceduralMeshFromStaticMeshComponent(StaticMeshComponent, 0, ProceduralMeshComponent, true);

	HitEffectParticleSystemComponent->SetTemplate(InDataTableRow->GunShotEffect);
	HitEffectParticleSystemComponent->Deactivate();

	SetData();

	ProceduralMeshComponent->AddImpulse(InImpulse);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProceduralMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHitAnyObject);
	HitEffectParticleSystemComponent->OnParticleSpawn.AddDynamic(this, &ThisClass::OnParticleSpawned);
}

void AProjectile::SetData()
{
	MeshType = MeshDataTableRow->MeshType;
	SliceParticleSystemComponent->SetTemplate(MeshDataTableRow->SliceEffect);
	SliceParticleSystemComponent->Deactivate();
}

void AProjectile::OnHitAnyObject(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	if (!Weapon)
	{
		HitEffectParticleSystemComponent->SetWorldLocation(Hit.Location);
		HitEffectParticleSystemComponent->Activate(true);
		ProceduralMeshComponent->SetSimulatePhysics(false);
	}
}

void AProjectile::OnParticleSpawned(FName EventName, float EmitterTime, FVector Location, FVector Velocity)
{
	UE_LOG(LogTemp, Warning, TEXT("ParticleSpawned"));
}
