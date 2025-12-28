// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontUI/Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UMyListDataObject_String;
class UFrontendCommonRotator;
class UFrontendCommonButtonBase;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTUI_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifiedReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;

private:

	UFUNCTION()
	void OnPreviousOptionButtonClicked();
	UFUNCTION()
	void OnNextOptionButtonClicked();
	
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonButtonBase* CommonButton_NextOption;

	UPROPERTY(Transient)
	UMyListDataObject_String* CachedOwningStringDataObject;
};

