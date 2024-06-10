// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("GrabComponent"));
	GrabComponent->SetupAttachment(GetRootComponent());

	GrabComponent->OnGrabbed.AddDynamic(this, &ThisClass::OnGrab);
	GrabComponent->OnReleaseGrabbed.AddDynamic(this, &ThisClass::OnReleaseGrab);
}

void AWeapon::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	VRWeaponDT = InDataTableRowHandle;

	if (VRWeaponDT.IsNull()) { return; }
	if (VRWeaponDT.RowName == NAME_None) { return; }

	FVRWeaponDataTableRow* VRWeaponDataTableRow = VRWeaponDT.GetRow<FVRWeaponDataTableRow>(TEXT(""));
	SetData(VRWeaponDataTableRow);
}

void AWeapon::SetData(const FVRWeaponDataTableRow* VRWeaponDataTableRow)
{
	if (!VRWeaponDataTableRow)
		return;
	if (VRWeaponDataTableRow->StaticMesh && VRWeaponDataTableRow->SkeletalMesh)
	{
		check(false);
		return;
	}
	else if (!VRWeaponDataTableRow->StaticMesh && !VRWeaponDataTableRow->SkeletalMesh)
	{
		check(false);
		return;
	}

	if (VRWeaponDataTableRow->StaticMesh != StaticMeshComponent->GetStaticMesh())
	{
		StaticMeshComponent->SetStaticMesh(VRWeaponDataTableRow->StaticMesh);
	}
	StaticMeshComponent->SetRelativeTransform(VRWeaponDataTableRow->MeshTransform, false, nullptr, ETeleportType::ResetPhysics);
	if (VRWeaponDataTableRow->SkeletalMesh != SkeletalMeshComponent->GetSkeletalMeshAsset())
	{
		SkeletalMeshComponent->SetSkeletalMesh(VRWeaponDataTableRow->SkeletalMesh);
	}
	SkeletalMeshComponent->SetAnimInstanceClass(VRWeaponDataTableRow->AnimClass);
	SkeletalMeshComponent->SetRelativeTransform(VRWeaponDataTableRow->MeshTransform, false, nullptr, ETeleportType::ResetPhysics);

	GrabComponent->SetRelativeTransform(VRWeaponDataTableRow->GrabComponentTransform);
	GrabComponent->SetData(VRWeaponDataTableRow);

	if (IsValid(VRWeaponDataTableRow->WeaponActorComponentClass))
	{
		if (WeaponActorComponent)
		{
			WeaponActorComponent->DestroyComponent();
			WeaponActorComponent = nullptr;
		}
		WeaponActorComponent = NewObject<UWeaponActorComponent>(this, VRWeaponDataTableRow->WeaponActorComponentClass);
		WeaponActorComponent->RegisterComponent();
		WeaponActorComponent->SetData(VRWeaponDataTableRow);
	}
	else if (IsValid(WeaponActorComponent))
	{
		WeaponActorComponent->DestroyComponent();
		WeaponActorComponent = nullptr;
	}
}

UAnimInstance* AWeapon::GetAnimInstance()
{
	if (!IsValid(SkeletalMeshComponent))
	{
		check(false);
		return nullptr;
	}

	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	check(AnimInstance);
	return AnimInstance;
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetData(VRWeaponDT);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SetData(VRWeaponDT);
}

void AWeapon::OnGrab()
{
	/*APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn);

	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	check(PlayerController);

	bool bBindAction = false;
	if (!InputComponent)
	{
		bBindAction = true;
	}

	EnableInput(PlayerController);*/
}

void AWeapon::OnReleaseGrab()
{
	/*APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn);

	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	check(PlayerController);

	DisableInput(PlayerController);*/
}

void AWeapon::OnTriggerStarted()
{
	if (!IsValid(WeaponActorComponent)) { return; }

	WeaponActorComponent->OnTriggerStarted();
}

void AWeapon::OnTriggerCompleted()
{
	if (!IsValid(WeaponActorComponent)) { return; }

	WeaponActorComponent->OnTriggerCompleted();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

