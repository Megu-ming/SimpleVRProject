// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/BulletDispenser.h"

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
	DelayTime = DispenserDataTableRow->DelayTime;
}

// Called every frame
void ABulletDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletDispenser::PlayAnimation()
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	check(AnimInstance);
	AnimInstance->Montage_Play(FireMontage);
}

