// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MISC/ProceduralEnum.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProceduralMesh.generated.h"

USTRUCT()
struct FProceduralMeshDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "MeshType")
	EProceduralMeshType MeshType = EProceduralMeshType::BackGround;

	UPROPERTY(EditAnywhere, Category = "ProceduralMesh\StaticMesh")
	UStaticMesh* StaticMesh;
		
	UPROPERTY(EditAnywhere, Category = "ProceduralMesh\Particles")
	UParticleSystem* SliceEffect;
};

UCLASS()
class SIMPLEVRPROJECT_API AProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMesh();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetData(FDataTableRowHandle& InProceduralMeshDataTableRowHandle);
	void SetData(FProceduralMeshDataTableRow* InDataTableRow);
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
	UPROPERTY(Category = "DataTable", EditAnywhere, meta = (RowType = "/Script/SIMPLEVRPROJECT.ProceduralMeshDataTableRow"))
	FDataTableRowHandle ProceduralMeshDT;
	FProceduralMeshDataTableRow* ProceduralMeshDataTableRow;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProceduralMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* SliceParticleSystemComponent;

	UPROPERTY(EditAnywhere)
	EProceduralMeshType MeshType = EProceduralMeshType::BackGround;
	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;
};
