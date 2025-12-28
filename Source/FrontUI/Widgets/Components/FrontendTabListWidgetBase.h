// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendTabListWidgetBase.generated.h"

class UFrontendCommonButtonBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTUI_API UFrontendTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:

	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);

private:

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FrontEnd Tab List Settings", meta=(AllowPrivateAccess="true", ClampMin = "1", ClampMax="10"))
	int32 DebugEditorPreviewCountTab = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FrontEnd Tab List Settings", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;
};
