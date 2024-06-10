// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponActorComponent.h"
#include "Weapon/Weapon.h"

FVRWeaponDataTableRow::FVRWeaponDataTableRow()
	: WeaponActorComponentClass(UWeaponActorComponent::StaticClass())
{}

// Sets default values for this component's properties
UWeaponActorComponent::UWeaponActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWeaponActorComponent::SetData(const FVRWeaponDataTableRow* InDataTableRow)
{
	VRWeapon = Cast<AWeapon>(GetOwner());
	check(VRWeapon);
	DataTableRow = InDataTableRow;
}

void UWeaponActorComponent::OnTriggerStarted()
{
}

void UWeaponActorComponent::OnTriggerCompleted()
{
}

// Called every frame
void UWeaponActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


