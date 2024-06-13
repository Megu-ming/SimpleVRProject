// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/BulletDispenser.h"

// Sets default values
ABulletDispenser::ABulletDispenser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScene"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	SetRootComponent(DefaultScene);
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABulletDispenser::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &ThisClass::PlayAnimation, DelayTime, true, 2.f);
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

