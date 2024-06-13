// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "BulletDispenser.generated.h"

UCLASS()
class SIMPLEVRPROJECT_API ABulletDispenser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletDispenser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayAnimation();

private:
	FTimerHandle AnimationTimerHandle;
	UPROPERTY(EditAnywhere)
	UAnimMontage* FireMontage = nullptr;
	UPROPERTY(EditAnywhere)
	float DelayTime = 5.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;

	
	//TArray<AProjectile*> Projectiles;
};
