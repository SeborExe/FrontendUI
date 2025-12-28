// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontUI/FunctionLibrary/FrontendFunctionLibrary.h"

#include "FrontUI/DeveloperSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta=(Categories="Frontend.Widget")) FGameplayTag InWidgetTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag), TEXT("Cannot find widget with tag: %s"), *InWidgetTag.ToString());
	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag InImageTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->OptionsScreenSoftImagesMap.Contains(InImageTag), TEXT("Cannot find Image with tag: %s"), *InImageTag.ToString());
	return FrontendDeveloperSettings->OptionsScreenSoftImagesMap.FindRef(InImageTag);
}
