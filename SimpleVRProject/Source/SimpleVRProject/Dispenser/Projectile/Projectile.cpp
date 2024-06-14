// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/Projectile/Projectile.h"
#include "Weapon/Weapon.h"

AProjectile::AProjectile()
{
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("/Script/Engine.DataTable'/Game/StartMap/DataTable/DT_Procedural.DT_Procedural'"));
		ensure(Asset.Object);
		DataTable = Asset.Object;
	}
	HitEffectParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffectParticleSystemComponent"));
	ProceduralMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	
	SetLifeSpan(5.f);
}

void AProjectile::Init(UStaticMesh* const Mesh, const FTransform& InTransform, const FVector& InImpulse, UParticleSystem* ParticleSystem)
{
	if (!IsValid(Mesh)) { return; }

	ProceduralMeshComponent->SetRelativeScale3D(InTransform.GetScale3D());
	ProceduralMeshComponent->SetRelativeRotation(InTransform.GetRotation());
	//ProceduralMeshComponent->AddImpulse(InImpulse);
	StaticMeshComponent->SetStaticMesh(Mesh);

	HitEffectParticleSystemComponent->SetTemplate(ParticleSystem);
	HitEffectParticleSystemComponent->Deactivate();

	SetData();
}

void AProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProceduralMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHitAnyObject);
	HitEffectParticleSystemComponent->OnParticleSpawn.AddDynamic(this, &ThisClass::OnParticleSpawned);
}

void AProjectile::SetData()
{
	Super::SetData();

	FProceduralMeshDataTableRow* Data = DataTable->FindRow<FProceduralMeshDataTableRow>(FName("Projectile"), TEXT(""));
	if (!Data) { ensure(false); return; }

	MeshType = Data->MeshType;
	SliceParticleSystemComponent->SetTemplate(Data->SliceEffect);
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
