// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/BasicInputDataConfig.h"
#include "InputMappingContext.h"
#include "InputAction.h"

UBasicInputDataConfig::UBasicInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/StartMap/Hand/Input/Basic/IMC_Basic.IMC_Basic'") };
		check(Asset.Succeeded());
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/Basic/IA_Move.IA_Move'") };
		check(Asset.Succeeded());
		Move = Asset.Object;
	}
}
