// Fill out your copyright notice in the Description page of Project Settings.


#include "Obejct/ProceduralMesh.h"
#include "KismetProceduralMeshLibrary.h"
#include "Weapon/Weapon.h"

// Sets default values
AProceduralMesh::AProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	ProceduralMeshComponent->bUseComplexAsSimpleCollision = false;
	ProceduralMeshComponent->SetSimulatePhysics(true);
	ProceduralMeshComponent->SetNotifyRigidBodyCollision(true);
	ProceduralMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	ProceduralMeshComponent->AddImpulse(FVector(200, 200, 200));

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	StaticMeshComponent->SetVisibility(false);
}

void AProceduralMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UKismetProceduralMeshLibrary::
		CopyProceduralMeshFromStaticMeshComponent(StaticMeshComponent, 0, ProceduralMeshComponent, true);
}

// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
	
	ProceduralMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

// Called every frame
void AProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralMesh::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AWeapon* Weapon = Cast<AWeapon>(OtherActor))
	{
		FVector PlanePosition = Hit.Location;
		FVector PlaneNormal = Weapon->GetSkeletalMeshComponent()->GetForwardVector();
		UProceduralMeshComponent* NewMesh;
		UKismetProceduralMeshLibrary::SliceProceduralMesh(ProceduralMeshComponent, PlanePosition, -PlaneNormal, true,
			NewMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, nullptr);
		
		NewMesh->bUseComplexAsSimpleCollision = false;
		NewMesh->SetSimulatePhysics(true);
		NewMesh->AddImpulse(FVector(200, 200, 200));
	}
}

