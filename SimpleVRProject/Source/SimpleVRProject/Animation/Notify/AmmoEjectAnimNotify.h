// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AmmoEjectAnimNotify.generated.h"

UCLASS()
class SIMPLEVRPROJECT_API UAmmoEjectAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITORONLY_DATA
protected:
	UPROPERTY(Category = "Editor\EjectCatridge", EditAnywhere, meta = (RowType = "/Script/SIMPLEVRPROJECT.ProjectileDataTableRow"))
	FDataTableRowHandle EditorPreviewBulletDataTable;
#endif

protected:
	static inline const FName AmmoEjectSocket = TEXT("AmmoEject");
	static inline const FName MuzzleFlashSocket = TEXT("MuzzleFlash");

};
