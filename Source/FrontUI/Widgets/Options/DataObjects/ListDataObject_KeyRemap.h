// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"

class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;

UCLASS()
class FRONTUI_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()

public:

	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings, UEnhancedPlayerMappableKeyProfile* InOwningPlayerMappableKeyProfile, ECommonInputType InDesiredInputType,
		const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	FSlateBrush GetIconFromCurrentKey() const;

	void BindNewInputKey(const FKey& InNewKey);

	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

private:

	FPlayerKeyMapping* GetOwningKeyMapping() const;

	bool IsKeyAlreadyBound(const FKey& InKey) const;

	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningUserSettings;
	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;
	FName CachedOwningMappingName;
	EPlayerMappableKeySlot CachedOwningMappingKeySlot;
	FText CachedDisplayCategory;

public:

	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const { return CachedDesiredInputKeyType; }
};
