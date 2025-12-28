// Fill out your copyright notice in the Description page of Project Settings.

#include "FrontUI/Widgets/Components/FrontendCommonListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "FrontUI/Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "FrontUI/Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_Collection.h"

UUserWidget& UFrontendCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}

bool UFrontendCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UListDataObject_Collection>();
}

#if WITH_EDITOR
void UFrontendCommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("DataListEntryMapping not set")));
	}
}
#endif
