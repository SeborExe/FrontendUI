// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontUI/Enums/FrontEnumTypes.h"
#include "FrontUI/Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UListDataObject_Base;
class UWidget_OptionsDetailsView;
class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;
class UFrontendCommonListView;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:

	UOptionsDataRegistry* GetOrCreateDataRegistry();

	UFUNCTION()
	void OnResetBoundActionTriggered();
	UFUNCTION()
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);
	UFUNCTION()
	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	UFUNCTION()
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	UFUNCTION()
	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	// START Bound Widgets
	UPROPERTY(meta=(BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTab;

	UPROPERTY(meta=(BindWidget))
	UWidget_OptionsDetailsView* DetailsView_ListEntryInfo;
	// END Bound Widgets

	UPROPERTY(meta=(BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;

	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "Fronted Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ResettableDataArray;

	bool bIsResettingData{false};
};
