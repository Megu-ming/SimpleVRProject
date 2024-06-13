// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMesh.generated.h"

UCLASS()
class SIMPLEVRPROJECT_API AProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMesh();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UProceduralMeshComponent* GetProceduralMeshComponent() { return ProceduralMeshComponent; }
	UStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProceduralMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;
};
