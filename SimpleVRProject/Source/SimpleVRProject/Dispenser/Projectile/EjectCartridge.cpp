// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/Projectile/EjectCartridge.h"

// Sets default values
AEjectCartridge::AEjectCartridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	StaticMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	StaticMeshComponent->SetSimulatePhysics(true);
}

void AEjectCartridge::Init(UStaticMesh* const Mesh, const FVector& InImpulse)
{
	if (!IsValid(Mesh)) { return; }

	StaticMeshComponent->SetStaticMesh(Mesh);
	StaticMeshComponent->AddImpulse(InImpulse);
}


