// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AmmoEjectAnimNotify.h"
#include "Dispenser/Projectile/Projectile.h"
#include "Dispenser/Projectile/EjectCartridge.h"
#include "Dispenser/BulletDispenser.h"

void UAmmoEjectAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) { return; }

	const FTransform AmmoEjectTransform = MeshComp->GetSocketTransform(AmmoEjectSocket);
	const FTransform MuzzleFlashTransform = MeshComp->GetSocketTransform(MuzzleFlashSocket);

#if WITH_EDITOR
	if (GIsEditor && !FApp::IsGame())
	{
		if (EditorPreviewBulletDataTable.IsNull()) { return; }

		FProjectileDataTableRow* ProjectileDT = EditorPreviewBulletDataTable.GetRow<FProjectileDataTableRow>(TEXT(""));
		if (!ProjectileDT) { return; }

		{
			const FTransform FinalTransform = ProjectileDT->EjectStaticMeshTransform * AmmoEjectTransform;

			AEjectCartridge* EjectCartridge = MeshComp->GetWorld()->SpawnActor<AEjectCartridge>
				(AEjectCartridge::StaticClass(), FinalTransform);

			const FVector RightVector = EjectCartridge->GetActorRightVector();
			const double RandomVector = FMath::RandRange(4.0, 10.0);
			const FVector Impulse = RightVector * RandomVector;

			EjectCartridge->Init(ProjectileDT->EjectStaticMesh, Impulse);
		}
		{
			AProjectile* Bullet = MeshComp->GetWorld()->SpawnActorDeferred<AProjectile>
				(AProjectile::StaticClass(), MuzzleFlashTransform);

			const FTransform FinalTransform = ProjectileDT->BulletProceduralMeshTransform * MuzzleFlashTransform;

			const FVector ForwardVector = Bullet->GetActorUpVector();
			const float BulletSpeed = ProjectileDT->ProjectileSpeed;
			const FVector Impulse = ForwardVector * BulletSpeed;

			Bullet->Init(ProjectileDT->BulletStaticMesh, ProjectileDT->BulletProceduralMeshTransform, Impulse);
			Bullet->FinishSpawning(FinalTransform, true);
		}
		return;
	}
#endif
	ABulletDispenser* Dispenser = Cast<ABulletDispenser>(OwnerActor);   
	check(Dispenser);
	if (EditorPreviewBulletDataTable.IsNull()) { return; }

	FProjectileDataTableRow* ProjectileDT = EditorPreviewBulletDataTable.GetRow<FProjectileDataTableRow>(TEXT(""));
	if (!ProjectileDT) { return; }

	{
		const FTransform FinalTransform = ProjectileDT->EjectStaticMeshTransform * AmmoEjectTransform;

		AEjectCartridge* EjectCartridge = MeshComp->GetWorld()->SpawnActor<AEjectCartridge>(AEjectCartridge::StaticClass(),
			FinalTransform);

		const FVector RightVector = EjectCartridge->GetActorRightVector();
		const double RandomVector = FMath::RandRange(50.0, 100.0);
		const FVector Impulse = RightVector * RandomVector;

		EjectCartridge->Init(ProjectileDT->EjectStaticMesh, Impulse);
	}
	{
		//const FTransform FinalTransform = ProjectileDT->BulletProceduralMeshTransform * MuzzleFlashTransform;

		AProjectile* Bullet = MeshComp->GetWorld()->SpawnActorDeferred<AProjectile>
			(AProjectile::StaticClass(), MuzzleFlashTransform);

		const FTransform FinalTransform = ProjectileDT->BulletProceduralMeshTransform * MuzzleFlashTransform;

		const FVector UpVector = Bullet->GetActorUpVector();
		const float BulletSpeed = ProjectileDT->ProjectileSpeed;
		const FVector Impulse = UpVector * BulletSpeed;

		Bullet->Init(ProjectileDT->BulletStaticMesh, ProjectileDT->BulletProceduralMeshTransform, Impulse);
		Bullet->FinishSpawning(FinalTransform, true);
	}
}
