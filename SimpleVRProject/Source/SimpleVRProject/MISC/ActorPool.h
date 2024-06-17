// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FActorPool
{
	FActorPool();
	~FActorPool();

	void Create(UWorld* World, TSubclassOf<AActor> Class, uint32 Count);
	void Destroy();

	template<class T>
	T* New(const FTransform& InTransform, TFunction<void(T* NewActor)>&& InitCallback = nullptr, bool bEnableCollision = true, AActor* Owner = nullptr, APawn* Instigator = nullptr)
	{
		AActor* Actor = nullptr;
		if (Pool.Num() == 0)
		{
			Actor = ActiveActors[0];
			Delete(Actor);
			Actor = nullptr;
		}
		Actor = Pool.Pop(false);
		ActiveActors.Push(Actor);
		Actor->SetOwner(Owner);
		Actor->SetInstigator(Instigator);

		T* ActorT = Cast<T>(Actor);
		check(ActorT);

		Actor->SetActorEnableCollision(bEnableCollision);
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorTickEnabled(true);
		const TSet<UActorComponent*>& Components = Actor->GetComponents();
		for (UActorComponent* It : Components)
		{
			if (It->HasBeenInitialized())
			{
				It->UninitializeComponent();
			}
		}

		Actor->SetActorTransform(InTransform, false, nullptr, ETeleportType::ResetPhysics);

		if (InitCallback)
		{
			InitCallback(ActorT);
		}

		for (UActorComponent* It : Components)
		{
			It->SetComponentTickEnabled(true);
			if (!It->HasBeenInitialized())
			{
				It->InitializeComponent();
			}
		}

		return ActorT;
	}

	void Delete(AActor* InActor)
	{
		const int32 Index = ActiveActors.Find(InActor);
		if (INDEX_NONE == Index)
		{
			check(false);
			return;
		}
		ActiveActors.RemoveAt(Index);
		Pool.Add(InActor);

		if (UFunction* Function = InActor->FindFunction(TEXT("OnActorPoolBeginDelete")))
		{
			InActor->ProcessEvent(Function, nullptr);
		}

		if (AActor* ParentActor = InActor->GetAttachParentActor())
		{
			InActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		InActor->SetActorEnableCollision(false);
		InActor->SetActorHiddenInGame(true);
		InActor->SetActorTickEnabled(false);
		const TSet<UActorComponent*>& Components = InActor->GetComponents();
		for (UActorComponent* It : Components)
		{
			It->SetComponentTickEnabled(false);
			if (It->HasBeenInitialized())
			{
				It->UninitializeComponent();
			}
		}
	}

	TArray<AActor*> Pool;

	// Pool에서 꺼내서 외부에서 처리되고 있는 Actor
	TArray<AActor*> ActiveActors;
};