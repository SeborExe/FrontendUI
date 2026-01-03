// Fill out your copyright notice in the Description page of Project Settings.

#include "FrontUI/DeveloperSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"	

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("No Valid Loading Screen in Loading Screen Settings"));

	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();
	return LoadedLoadingScreenWidget;
}
