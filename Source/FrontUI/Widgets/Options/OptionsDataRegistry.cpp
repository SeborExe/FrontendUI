// Fill out your copyright notice in the Description page of Project Settings.

#include "FrontUI/Widgets/Options/OptionsDataRegistry.h"

#include "OptionsDataInteractionHelper.h"
#include "DataObjects/ListDataObject_Collection.h"
#include "DataObjects/ListDataObject_Scalar.h"
#include "DataObjects/MyListDataObject_String.h"
#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "FrontUI/DeveloperSettings/FrontendGameUserSettings.h"
#include "Internationalization/StringTableRegistry.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/Blueprints/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InputInOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPointer = RegisteredOptionsTabCollections.FindByPredicate([InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
	{
		return AvailableTabCollection->GetDataID() == InSelectedTabID;
	});

	checkf(FoundTabCollectionPointer, TEXT("NO valid tab found under ID: %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPointer;

	TArray<UListDataObject_Base*> AllChildListItems;
	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData) continue;

		AllChildListItems.Add(ChildListData);
		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursive(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursive(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutChildren) const
{
	if (!InParentData || !InParentData->HasAnyChildListData()) return;

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData) continue;

		OutChildren.Add(SubChildListData);
		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursive(SubChildListData, OutChildren);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTagCollection =  NewObject<UListDataObject_Collection>();
	GameplayTagCollection->SetDataID(FName("GameplayTagCollection"));
	GameplayTagCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = MakeShared<FOptionsDataInteractionHelper>(
		GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty));*/

	UMyListDataObject_String* GameDifficulty = NewObject<UMyListDataObject_String>();
	GameDifficulty->SetDataID(FName("GameDifficulty"));
	GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
	GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
	GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
	GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
	GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
	GameDifficulty->AddDynamicOption(TEXT("Insane"), FText::FromString(TEXT("Insane")));
	GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
	GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
	GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
	GameDifficulty->SetShouldApplySettingsImmediately(true);
	
	GameplayTagCollection->AddChildListData(GameDifficulty);
	
	RegisteredOptionsTabCollections.Add(GameplayTagCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTagCollection =  NewObject<UListDataObject_Collection>();
	AudioTagCollection->SetDataID(FName("AudioTagCollection"));
	AudioTagCollection->SetDataDisplayName(FText::FromString("Audio"));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString("Volume"));
		
		AudioTagCollection->AddChildListData(VolumeCategoryCollection);

		//Overall volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString("Overall Volume"));
			OverallVolume->SetDescriptionRichText(FText::FromString("This is description for overall volume"));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		//Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString("Music Volume"));
			MusicVolume->SetDescriptionRichText(FText::FromString("This is description for Music volume"));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		//Sound Effect VFX
		{
			UListDataObject_Scalar* SoundsVFX = NewObject<UListDataObject_Scalar>();
			SoundsVFX->SetDataID(FName("SoundVFX"));
			SoundsVFX->SetDataDisplayName(FText::FromString("Sound VFX Volume"));
			SoundsVFX->SetDescriptionRichText(FText::FromString("This is description for Sound VFX volume"));
			SoundsVFX->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundsVFX->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundsVFX->SetSliderStepSize(0.01f);
			SoundsVFX->SetDefaultValueFromString(LexToString(1.f));
			SoundsVFX->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundsVFX->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			SoundsVFX->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundVFXVolume));
			SoundsVFX->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundVFXVolume));
			SoundsVFX->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(SoundsVFX);
		}
	}

	//Sound Category
	{
		UListDataObject_Collection* SoundCategegoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategegoryCollection->SetDataID(FName("SoundCategoriesCollection"));
		SoundCategegoryCollection->SetDataDisplayName(FText::FromString("Sound"));

		AudioTagCollection->AddChildListData(SoundCategegoryCollection);

		//Allow background Audio
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString("Allow Background Audio"));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString("Disabled"));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);

			SoundCategegoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		//Use HDR Audio
		{
			UListDataObject_StringBool* UseHDRAudio = NewObject<UListDataObject_StringBool>();
			UseHDRAudio->SetDataID(FName("UseHDRAudio"));
			UseHDRAudio->SetDataDisplayName(FText::FromString("Use HDR Audio Mode"));
			UseHDRAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			UseHDRAudio->OverrideTrueDisplayText(FText::FromString("Disabled"));
			UseHDRAudio->SetFalseAsDefaultValue();
			UseHDRAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudio));
			UseHDRAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudio));
			UseHDRAudio->SetShouldApplySettingsImmediately(true);

			SoundCategegoryCollection->AddChildListData(UseHDRAudio);
		}
	}
	
	RegisteredOptionsTabCollections.Add(AudioTagCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTagCollection =  NewObject<UListDataObject_Collection>();
	VideoTagCollection->SetDataID(FName("VideoTagCollection"));
	VideoTagCollection->SetDataDisplayName(FText::FromString("Video"));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString("Display"));

		VideoTagCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescription PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc([]()->bool
		{
			const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
			return !bIsInEditor;
		});
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This settings can only been adjusted in packaged build/</>"));

		//Window mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString("Window Mode"));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Full Screen Mode")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);
			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		//Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);
			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescription WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc([CreatedWindowMode]()->bool
			{
				const bool bIsBorderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;
				return !bIsBorderlessWindow;
			});
			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Screen Resolution is not adjustable when 'Window Mode' is set to Borderless Window. The value must match with the maximum allowed resolution.</>"));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);
			
			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	//Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString("Graphics"));

		VideoTagCollection->AddChildListData(GraphicsCategoryCollection);

		//Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString("Brightness"));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); // Default value in Unreal is 2.2f
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInteger* CreatedOverallQuality = nullptr;
		
		//Overall Quality
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString("Overall Quality"));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, FText::FromString("Low"));
			OverallQuality->AddIntegerOption(1, FText::FromString("Medium"));
			OverallQuality->AddIntegerOption(2, FText::FromString("High"));
			OverallQuality->AddIntegerOption(3, FText::FromString("Epic"));
			OverallQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);
			
			GraphicsCategoryCollection->AddChildListData(OverallQuality);
			CreatedOverallQuality = OverallQuality;
		}

		//Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString("3D Resolution"));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		//Global Illumination
		{
			UListDataObject_StringInteger* GlobalIllumination = NewObject<UListDataObject_StringInteger>();
			GlobalIllumination->SetDataID(FName("GlobalIllumination"));
			GlobalIllumination->SetDataDisplayName(FText::FromString("Global Illumination"));
			GlobalIllumination->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
			GlobalIllumination->AddIntegerOption(0, FText::FromString("Low"));
			GlobalIllumination->AddIntegerOption(1, FText::FromString("Medium"));
			GlobalIllumination->AddIntegerOption(2, FText::FromString("High"));
			GlobalIllumination->AddIntegerOption(3, FText::FromString("Epic"));
			GlobalIllumination->AddIntegerOption(4, FText::FromString("Cinematic"));
			GlobalIllumination->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIllumination->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIllumination->SetShouldApplySettingsImmediately(true);

			GlobalIllumination->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(GlobalIllumination);
			
			GraphicsCategoryCollection->AddChildListData(GlobalIllumination);
		}

		//Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString("Shadow Quality"));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, FText::FromString("Low"));
			ShadowQuality->AddIntegerOption(1, FText::FromString("Medium"));
			ShadowQuality->AddIntegerOption(2, FText::FromString("High"));
			ShadowQuality->AddIntegerOption(3, FText::FromString("Epic"));
			ShadowQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);
			
			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		//AntiAliasing Quality
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString("AntiAliasing Quality"));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString("Low"));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString("Medium"));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString("High"));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString("Epic"));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);
			
			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString("View Distance Quality"));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString("Near"));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString("Medium"));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString("Far"));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString("Very Far"));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);
			
			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString("Texture Quality"));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, FText::FromString("Low"));
			TextureQuality->AddIntegerOption(1, FText::FromString("Medium"));
			TextureQuality->AddIntegerOption(2, FText::FromString("Medium"));
			TextureQuality->AddIntegerOption(3, FText::FromString("Epic"));
			TextureQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(TextureQuality);
			
			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		//Visual Effect Quality
		{
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(FText::FromString("Visual Effect Quality"));
			VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
			VisualEffectQuality->AddIntegerOption(0, FText::FromString("Low"));
			VisualEffectQuality->AddIntegerOption(1, FText::FromString("Medium"));
			VisualEffectQuality->AddIntegerOption(2, FText::FromString("Medium"));
			VisualEffectQuality->AddIntegerOption(3, FText::FromString("Epic"));
			VisualEffectQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);
			
			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		//Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString("Reflection Quality"));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, FText::FromString("Low"));
			ReflectionQuality->AddIntegerOption(1, FText::FromString("Medium"));
			ReflectionQuality->AddIntegerOption(2, FText::FromString("Medium"));
			ReflectionQuality->AddIntegerOption(3, FText::FromString("Epic"));
			ReflectionQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);
			
			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		//Post Processing Quality
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString("Post Processing Quality"));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString("Low"));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString("Medium"));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString("Medium"));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString("Epic"));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString("Cinematic"));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);
			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);
			
			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	//Advanced Graphics Category
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString("Advanced Graphics"));

		VideoTagCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		//Vertical Sync
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString("V-Sync"));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplySettingsImmediately(true);

			FOptionsDataEditConditionDescription FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc([CreatedWindowMode]()->bool
			{
				return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
			});
			FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature only works if the 'Window mode' is set to 'Full Screen'.</>"));
			FullScreenOnlyCondition.SetDisabledRichReason(TEXT("false"));

			VerticalSync->AddEditCondition(FullScreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		//Frame Rate Limit
		{
			UMyListDataObject_String* FrameRateLimit = NewObject<UMyListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(GET_DESCRIPTION("FrameRateLimitDescKey"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString("30 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString("60 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString("90 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString("120 FPS"));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString("No Limit"));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediately(true);

			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}
	
	RegisteredOptionsTabCollections.Add(VideoTagCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTagCollection =  NewObject<UListDataObject_Collection>();
	ControlTagCollection->SetDataID(FName("ControlTagCollection"));
	ControlTagCollection->SetDataDisplayName(FText::FromString("Control"));
	
	RegisteredOptionsTabCollections.Add(ControlTagCollection);
}
