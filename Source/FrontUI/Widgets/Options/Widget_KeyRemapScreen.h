// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "FrontUI/Widgets/Widget_ActivatableBase.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:

	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /*CanceledReason*/)
	FOnKeyRemapScreenKeySelectCanceledDelegate OnKeyRemapScreenKeySelectCanceled;

protected:

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:

	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectedCanceled(const FString& CanceledReason);

	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback); //Delay a tick to make a sure the input key is captured before calling Predeactive callbacl

	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_Message;

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;

	ECommonInputType CachedDesiredInputType;
};
