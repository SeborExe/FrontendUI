// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;	

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

private:

	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend button", meta=(AllowPrivateAccess="true"))
	bool bUseUpperCaseForButtonText = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Frontend button", meta=(AllowPrivateAccess="true"))
	FText ButtonDescriptionText;
};
