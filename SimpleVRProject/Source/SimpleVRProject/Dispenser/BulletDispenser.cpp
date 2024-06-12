// Fill out your copyright notice in the Description page of Project Settings.


#include "Dispenser/BulletDispenser.h"

// Sets default values
ABulletDispenser::ABulletDispenser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletDispenser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

