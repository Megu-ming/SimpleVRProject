// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EjectCartridge.generated.h"

UCLASS()
class SIMPLEVRPROJECT_API AEjectCartridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEjectCartridge();

	void Init(UStaticMesh* const Mesh, const FVector& InImpulse);

protected:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

};
