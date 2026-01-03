// Fill out your copyright notice in the Description page of Project Settings.

#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"

void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings,UEnhancedPlayerMappableKeyProfile* InOwningPlayerMappableKeyProfile,
                                                ECommonInputType InDesiredInputType, const FPlayerKeyMapping& InOwningPlayerKeyMapping)
{
	CachedOwningUserSettings = InOwningInputUserSettings;
	CachedOwningKeyProfile = InOwningPlayerMappableKeyProfile;
	CachedDesiredInputKeyType = InDesiredInputType;
	CachedOwningMappingName = InOwningPlayerKeyMapping.GetMappingName();
	CachedOwningMappingKeySlot = InOwningPlayerKeyMapping.GetSlot();
	CachedDisplayCategory = InOwningPlayerKeyMapping.GetDisplayCategory();
}

FSlateBrush UListDataObject_KeyRemap::GetIconFromCurrentKey() const
{
	check(CachedOwningUserSettings);
	
	FSlateBrush FoundBrush;
	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedOwningUserSettings->GetLocalPlayer());
	check(CommonInputSubsystem);
	
	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		FoundBrush, GetOwningKeyMapping()->GetCurrentKey(), CachedDesiredInputKeyType, CommonInputSubsystem->GetCurrentGamepadName()
	);

	if (!bHasFoundBrush)
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty Brush: %s"), *GetOwningKeyMapping()->GetCurrentKey().GetDisplayName().ToString())
	}
	
	return FoundBrush;
}

void UListDataObject_KeyRemap::BindNewInputKey(const FKey& InNewKey)
{
	check(CachedOwningUserSettings);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningMappingKeySlot;
	KeyArgs.NewKey = InNewKey;

	if (IsKeyAlreadyBound(InNewKey)) return;

	FGameplayTagContainer Container;
	CachedOwningUserSettings->MapPlayerKey(KeyArgs, Container);
	CachedOwningUserSettings->SaveSettings();

	NotifyListDataModified(this);
}

bool UListDataObject_KeyRemap::HasDefaultValue() const
{
	return GetOwningKeyMapping()->GetDefaultKey().IsValid();
}

bool UListDataObject_KeyRemap::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && GetOwningKeyMapping()->IsCustomized() && !IsKeyAlreadyBound(GetOwningKeyMapping()->GetDefaultKey());
}

bool UListDataObject_KeyRemap::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		check(CachedOwningUserSettings);
		GetOwningKeyMapping()->ResetToDefault();
		CachedOwningUserSettings->SaveSettings();

		NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
		return true;
	}

	return false;
}

FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyMapping() const
{
	check(CachedOwningKeyProfile);
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningMappingKeySlot;

	return CachedOwningKeyProfile->FindKeyMapping(KeyArgs);
}

bool UListDataObject_KeyRemap::IsKeyAlreadyBound(const FKey& InKey) const
{
    if (!CachedOwningUserSettings)
    {
       return false;
    }

    const TMap<FString, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& KeyProfiles = 
       CachedOwningUserSettings->GetAllAvailableKeyProfiles();

    for (const auto& ProfilePair : KeyProfiles)
    {
       const UEnhancedPlayerMappableKeyProfile* Profile = ProfilePair.Value;
       if (!Profile)
       {
          continue;
       }

       const TMap<FName, FKeyMappingRow>& PlayerKeyMappings = Profile->GetPlayerMappingRows();
       for (const auto& MappingPair : PlayerKeyMappings)
       {
          const FKeyMappingRow& MappingRow = MappingPair.Value;
            
          for (const FPlayerKeyMapping& KeyMapping : MappingRow.Mappings)
          {
             bool bIsCurrentMapping = (MappingPair.Key == CachedOwningMappingName &&  KeyMapping.GetSlot() == CachedOwningMappingKeySlot);
                
             if (bIsCurrentMapping)
             {
                continue;
             }
          	
          	if (KeyMapping.GetCurrentKey() == InKey && KeyMapping.GetDisplayCategory().EqualTo(CachedDisplayCategory))
             {
                return true;
             }
          }
       }
    }
	
    return false;
}