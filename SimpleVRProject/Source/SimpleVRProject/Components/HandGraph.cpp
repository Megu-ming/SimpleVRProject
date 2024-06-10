// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HandGraph.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Animation/VRHandAnimInstance.h"
#include "Character/VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Components/VRHandSkeletalMeshComponent.h"

UVRHandsAnimationInputDataConfig::UVRHandsAnimationInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/StartMap/Hand/Input/HandAnim/IMC_Hands.IMC_Hands'") };
		check(Asset.Object);
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_Grab_Left.IA_Grab_Left'") };
		check(Asset.Object);
		IA_Grab_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_Grab_Right.IA_Grab_Right'") };
		check(Asset.Object);
		IA_Grab_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_Point_Left.IA_Point_Left'") };
		check(Asset.Object);
		IA_Point_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_Point_Right.IA_Point_Right'") };
		check(Asset.Object);
		IA_Point_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_IndexCurl_Left.IA_IndexCurl_Left'") };
		check(Asset.Object);
		IA_IndexCurl_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_IndexCurl_Right.IA_IndexCurl_Right'") };
		check(Asset.Object);
		IA_IndexCurl_Right = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_Thumb_Left.IA_Thumb_Left'") };
		check(Asset.Object);
		IA_Thumb_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/IA_Thumb_Right.IA_Thumb_Right'") };
		check(Asset.Object);
		IA_Thumb_Right = Asset.Object;
	}
}

UVRHandsInputDataConfig::UVRHandsInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/StartMap/Hand/Input/HandAnim/HandInput/IMC_Hands_Input.IMC_Hands_Input'") };
		check(Asset.Object);
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/HandInput/IA_Grab_Left_Input.IA_Grab_Left_Input'") };
		check(Asset.Object);
		IA_Grab_Left = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/StartMap/Hand/Input/HandAnim/HandInput/IA_Grab_Right_Input.IA_Grab_Right_Input'") };
		check(Asset.Object);
		IA_Grab_Right = Asset.Object;
	}
}

void UHandGraph::SetupPlayerInputComponent(UMotionControllerComponent* InMotionControllerComponent, UEnhancedInputComponent* InputComponent)
{
	MotionControllerComponent = InMotionControllerComponent;
	check(MotionControllerComponent);

	AVRCharacter* Character = Cast<AVRCharacter>(MotionControllerComponent->GetOwner());
	check(Character);

	const UVRHandsAnimationInputDataConfig* VRInputDataConfig = GetDefault<UVRHandsAnimationInputDataConfig>();

	HandType = MotionControllerComponent->MotionSource == LeftGrip ? EHandType::Left : EHandType::Right;
	VRHandAnimInstance = HandType == EHandType::Left ?
		Cast<UVRHandAnimInstance>(Character->LeftHand->GetAnimInstance()) : Cast<UVRHandAnimInstance>(Character->RightHand->GetAnimInstance());
	check(VRHandAnimInstance);

	UInputAction* IA_Grab = HandType == EHandType::Left ? VRInputDataConfig->IA_Grab_Left : VRInputDataConfig->IA_Grab_Right;
	UInputAction* IA_Point = HandType == EHandType::Left ? VRInputDataConfig->IA_Point_Left : VRInputDataConfig->IA_Point_Right;
	UInputAction* IA_IndexCurl = HandType == EHandType::Left ? VRInputDataConfig->IA_IndexCurl_Left : VRInputDataConfig->IA_IndexCurl_Right;
	UInputAction* IA_Thumb = HandType == EHandType::Left ? VRInputDataConfig->IA_Thumb_Left : VRInputDataConfig->IA_Thumb_Right;

	InputComponent->BindAction(IA_Grab, ETriggerEvent::Triggered, this, &ThisClass::OnGrabTriggered);
	InputComponent->BindAction(IA_Grab, ETriggerEvent::Canceled, this, &ThisClass::OnGrabCompleted);
	InputComponent->BindAction(IA_Grab, ETriggerEvent::Completed, this, &ThisClass::OnGrabCompleted);

	InputComponent->BindAction(IA_Point, ETriggerEvent::Started, this, &ThisClass::OnPointStarted);
	InputComponent->BindAction(IA_Point, ETriggerEvent::Canceled, this, &ThisClass::OnPointStarted);
	InputComponent->BindAction(IA_Point, ETriggerEvent::Completed, this, &ThisClass::OnPointCompleted);

	InputComponent->BindAction(IA_IndexCurl, ETriggerEvent::Triggered, this, &ThisClass::OnIndexCurlTriggered);
	InputComponent->BindAction(IA_IndexCurl, ETriggerEvent::Canceled, this, &ThisClass::OnIndexCurlCompleted);
	InputComponent->BindAction(IA_IndexCurl, ETriggerEvent::Completed, this, &ThisClass::OnIndexCurlCompleted);

	InputComponent->BindAction(IA_Thumb, ETriggerEvent::Started, this, &ThisClass::OnThumbStarted);
	InputComponent->BindAction(IA_Thumb, ETriggerEvent::Canceled, this, &ThisClass::OnThumbStarted);
	InputComponent->BindAction(IA_Thumb, ETriggerEvent::Completed, this, &ThisClass::OnThumbCompleted);
}

void UHandGraph::OnGrabTriggered(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	VRHandAnimInstance->PoseAlphaGrab = ActionValue;
}

void UHandGraph::OnGrabCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaGrab = 0.f;
}

void UHandGraph::OnPointStarted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaPoint = 0.f;
}

void UHandGraph::OnPointCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaPoint = 1.f;
}

void UHandGraph::OnIndexCurlTriggered(const FInputActionValue& InputActionValue)
{
	const float ActionValue = InputActionValue.Get<float>();
	VRHandAnimInstance->PoseAlphaIndexCurl = ActionValue;
}

void UHandGraph::OnIndexCurlCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaIndexCurl = 0.f;
}

void UHandGraph::OnThumbStarted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaThumb = 0.f;
}

void UHandGraph::OnThumbCompleted(const FInputActionValue& InputActionValue)
{
	VRHandAnimInstance->PoseAlphaThumb = 1.f;
}


