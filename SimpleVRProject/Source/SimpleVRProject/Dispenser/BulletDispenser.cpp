// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/BulletDispenser.h"
#include "Projectile/EjectCartridge.h"
#include "Projectile/Projectile.h"

// Sets default values
ABulletDispenser::ABulletDispenser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	SetRootComponent(DefaultScene);
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

void ABulletDispenser::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetData(DispenserDT);
}

// Called when the game starts or when spawned
void ABulletDispenser::BeginPlay()
{
	Super::BeginPlay();
	
	EjectCartridgePool.Create(GetWorld(), AEjectCartridge::StaticClass(), 5);
	ProjectilePool.Create(GetWorld(), AProjectile::StaticClass(), 5);
	//EffectPool.Create(GetWorld(), AEffect::StaticClass(), 10);
	SetData(DispenserDT);
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &ThisClass::PlayAnimation, DelayTime, true, 2.f);
}

void ABulletDispenser::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DispenserDT = InDataTableRowHandle;

	if (DispenserDT.IsNull()) { return; }
	if (DispenserDT.RowName == NAME_None) { return; }

	DispenserDataTableRow = DispenserDT.GetRow<FDispenserDataTableRow>(TEXT(""));
	SetData(DispenserDataTableRow);
}

void ABulletDispenser::SetData(const FDispenserDataTableRow* InDispenserDataTableRow)
{
	DispenserDataTableRow = InDispenserDataTableRow;

	FireMontage = DispenserDataTableRow->FireMontage;
	SkeletalMeshComponent->SetSkeletalMeshAsset(DispenserDataTableRow->SkeletalMesh);
	SkeletalMeshComponent->SetAnimInstanceClass(DispenserDataTableRow->AnimInstance);
	DelayTime = DispenserDataTableRow->DelayTime;
}

// Called every frame
void ABulletDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletDispenser::SpawnEjectCartridge(const FTransform& InEjectTransform)
{
	EjectCartridgePool.New<AEjectCartridge>(InEjectTransform,
		[this, InEjectTransform](AEjectCartridge* NewActor)
		{
			if (DispenserDataTableRow->ProjectileDataTable.IsNull())
			{
				check(false);
				return;
			}

			FProjectileDataTableRow* ProjectileDataTableRow = DispenserDataTableRow->ProjectileDataTable.GetRow<FProjectileDataTableRow>(TEXT(""));
			if (!ProjectileDataTableRow)
			{
				check(false);
				return;
			}

			const FVector RightVector = NewActor->GetActorRightVector();
			const double RandomVector = FMath::RandRange(20.0, 30.0);
			const FVector Impulse = RightVector * RandomVector;

			NewActor->Init(ProjectileDataTableRow->EjectStaticMesh, Impulse);
		}
	);
}

void ABulletDispenser::SpawnProjectile(const FTransform& InTransform)
{
	ProjectilePool.New<AProjectile>(InTransform,
		[this, InTransform](AProjectile* NewActor)
		{
			if (DispenserDataTableRow->ProjectileDataTable.IsNull())
			{
				check(false);
				return;
			}

			FProjectileDataTableRow* ProjectileDataTableRow = DispenserDataTableRow->ProjectileDataTable.GetRow<FProjectileDataTableRow>(TEXT(""));
			FProceduralMeshDataTableRow* ProceduralMeshDataTableRow = DispenserDataTableRow->ProjectileMeshTypeDataTable.GetRow<FProceduralMeshDataTableRow>(TEXT(""));
			if (!ProjectileDataTableRow)
			{
				check(false);
				return;
			}
			if (!ProceduralMeshDataTableRow)
			{
				check(false);
				return;
			}

			/*UPrimitiveComponent* PrimitiveComponent = IsValid(SkeletalMeshComponent->GetSkeletalMeshAsset()) ?
				Cast<UPrimitiveComponent>(SkeletalMeshComponent) : StaticMeshComponent;
			FTransform Transform = VRWeaponDataTableRow->MeshTransform.Inverse() * PrimitiveComponent->GetComponentTransform();*/
			AProjectile* Projectile = Cast<AProjectile>(NewActor);
			if (!Projectile) { ensure(false); return; }
			Projectile->GetProceduralMeshComponent()->SetSimulatePhysics(true);

			const FVector ForwardVector = InTransform.GetUnitAxis(EAxis::X);
			const float BulletSpeed = ProjectileDataTableRow->ProjectileSpeed;
			const float RandSpeed = FMath::RandRange(BulletSpeed - 5, BulletSpeed);
			const FVector Impulse = ForwardVector * RandSpeed;

			UE_LOG(LogTemp, Warning, TEXT("SetProjectileData %s"), *NewActor->GetName());
			NewActor->Init(ProjectileDataTableRow, ProceduralMeshDataTableRow, Impulse);

		}, true, this
	);
}

void ABulletDispenser::PlayAnimation()
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->Montage_Play(FireMontage);
}

