#include "ActorPool.h"

FActorPool::FActorPool()
{
}

FActorPool::~FActorPool()
{
	Destroy();
}

void FActorPool::Create(UWorld* World, TSubclassOf<AActor> Class, uint32 Count)
{
	if (!Pool.IsEmpty())
	{
		check(false);
		return;
	}

	Pool.Reserve(Count);
	ActiveActors.Reserve(Count);

	FTransform DefaultTransform;
	for (uint32 i = 0; i < Count; ++i)
	{
		AActor* SpawnedActor = World->SpawnActorDeferred<AActor>(Class, DefaultTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedActor->SetActorEnableCollision(false);
		SpawnedActor->SetActorHiddenInGame(true);
		SpawnedActor->SetActorTickEnabled(false);

		SpawnedActor->FinishSpawning(DefaultTransform, true);

		/*const TSet<UActorComponent*>& Components = SpawnedActor->GetComponents();
		for (UActorComponent* It : Components)
		{
			It->SetComponentTickEnabled(false);
		}*/

		Pool.Add(SpawnedActor);
	}
}

void FActorPool::Destroy()
{
	for (auto* It : ActiveActors)
	{
		It->Destroy();
	}
	for (auto* It : Pool)
	{
		It->Destroy();
	}
	Pool.Empty();
	ActiveActors.Empty();
}
