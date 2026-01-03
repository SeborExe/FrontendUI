// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontUI/Widgets/Options/ListEntries/Widget_ListEntry_KeyRemap.h"

#include "FrontUI/FunctionLibrary/FrontendFunctionLibrary.h"
#include "FrontUI/Subsystem/FrontendUISubsystem.h"
#include "FrontUI/Tags/FrontendGameplayTags.h"
#include "FrontUI/Widgets/Components/FrontendCommonButtonBase.h"
#include "FrontUI/Widgets/Options/Widget_KeyRemapScreen.h"
#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);
	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifiedReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(FrontendGameplyTags::Frontend_WidgetStack_Modal,
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplyTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);
				
				if (CachedOwningKeyRemapDataObject)
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
			}
		});
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject) return;
	
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::Ok, FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("The key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" is already set to default")),
			[](EConfirmScreenButtonType ClickedButton){});
		return;
	}

	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::YesNo, FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(TEXT("Are you sure you want to reset the binding for: ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("?")),
		[this](EConfirmScreenButtonType ClickedButton)
		{
			if (ClickedButton == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}
		});
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(EConfirmScreenType::Ok,
		FText::FromString("Key Remap"), FText::FromString(CanceledReason), [](EConfirmScreenButtonType CLickedButton){});
}
