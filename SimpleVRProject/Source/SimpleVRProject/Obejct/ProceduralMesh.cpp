// Fill out your copyright notice in the Description page of Project Settings.


#include "Obejct/ProceduralMesh.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Weapon/Weapon.h"

// Sets default values
AProceduralMesh::AProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SliceParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SliceParticleSystemComponent"));
	
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint1"));
	PhysicsConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint2"));
	PhysicsConstraint3 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint3"));

	SetRootComponent(ProceduralMeshComponent);
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	SliceParticleSystemComponent->SetupAttachment(GetRootComponent());
	{
		PhysicsConstraint->SetupAttachment(GetRootComponent());
		PhysicsConstraint1->SetupAttachment(GetRootComponent());
		PhysicsConstraint2->SetupAttachment(GetRootComponent());
		PhysicsConstraint3->SetupAttachment(GetRootComponent());

		PhysicsConstraint->ConstraintActor1 = this;
		PhysicsConstraint1->ConstraintActor1 = this;
		PhysicsConstraint2->ConstraintActor1 = this;
		PhysicsConstraint3->ConstraintActor1 = this;
	}

	ProceduralMeshComponent->bUseComplexAsSimpleCollision = false;
	ProceduralMeshComponent->SetSimulatePhysics(true);
	ProceduralMeshComponent->SetNotifyRigidBodyCollision(true);
	ProceduralMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	StaticMeshComponent->SetVisibility(false);
}

void AProceduralMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UMaterialInterface* MaterialInstance = ProceduralMeshComponent->GetMaterial(0);
	DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialInstance);

	SetData(ProceduralMeshDT);
	
	UKismetProceduralMeshLibrary::
		CopyProceduralMeshFromStaticMeshComponent(StaticMeshComponent, 0, ProceduralMeshComponent, true);
}

// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();

	//SetData();
	ProceduralMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

	
}

void AProceduralMesh::SetData(FDataTableRowHandle& InProceduralMeshDataTableRowHandle)
{
	ProceduralMeshDT = InProceduralMeshDataTableRowHandle;

	if (ProceduralMeshDT.IsNull()) { return; }
	if (ProceduralMeshDT.RowName == NAME_None) { return; }

	ProceduralMeshDataTableRow = ProceduralMeshDT.GetRow<FProceduralMeshDataTableRow>(TEXT(""));
	SetData(ProceduralMeshDataTableRow);
}

void AProceduralMesh::SetData(FProceduralMeshDataTableRow* InDataTableRow)
{
	if (!InDataTableRow) { ensure(false); return; }

	MeshType = InDataTableRow->MeshType;
	StaticMeshComponent->SetStaticMesh(InDataTableRow->StaticMesh);
	SliceParticleSystemComponent->SetTemplate(InDataTableRow->SliceEffect);
	SliceParticleSystemComponent->Deactivate();
	if (!bUseConstraintComponent)
	{
		PhysicsConstraint->Deactivate();
		PhysicsConstraint1->Deactivate();
		PhysicsConstraint2->Deactivate();
		PhysicsConstraint3->Deactivate();
	}
	else
	{
		PhysicsConstraint->ComponentName1 = FConstrainComponentPropName(TEXT("ProceduralMeshComponent"));
		PhysicsConstraint1->ComponentName1 = FConstrainComponentPropName(TEXT("ProceduralMeshComponent"));
		PhysicsConstraint2->ComponentName1 = FConstrainComponentPropName(TEXT("ProceduralMeshComponent"));
		PhysicsConstraint3->ComponentName1 = FConstrainComponentPropName(TEXT("ProceduralMeshComponent"));
	}
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
		SliceParticleSystemComponent->SetWorldLocation(Hit.Location);
		SliceParticleSystemComponent->Activate();
		UKismetProceduralMeshLibrary::SliceProceduralMesh(ProceduralMeshComponent, PlanePosition, -PlaneNormal, true,
			NewMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, DynamicMaterial);
		
		NewMesh->bUseComplexAsSimpleCollision = false;
		NewMesh->SetSimulatePhysics(true);
		NewMesh->AddImpulse(FVector(200, 200, 200));
	}
}

