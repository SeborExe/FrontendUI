// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontUI/DeveloperSettings/FrontendGameUserSettings.h"

#include "FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: OverallVolume(1.f), MusicVolume(1.f), SoundVFXVolume(1.f), bAllowBackgroundAudio(false), bUseHDRAudio(false)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendGameUserSettings::SetOverallVolume(float InVolume)
{
	UWorld* InAudioWorld = nullptr;
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
 
	if (GEngine)
	{
		InAudioWorld = GEngine->GetCurrentPlayWorld();
	}
 
	if (!InAudioWorld || !FrontendDeveloperSettings)
	{
		return;
	}
 
	USoundClass* MasterSoundClass = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->MasterSoundClass.TryLoad())
	{
		MasterSoundClass = CastChecked<USoundClass>(LoadedObject);
	}
 
	USoundMix* DefaultSoundMix = nullptr;
	if (UObject* LoadedObject = FrontendDeveloperSettings->DefaultSoundMix.TryLoad())
	{
		DefaultSoundMix = CastChecked<USoundMix>(LoadedObject);
	}
 
	OverallVolume = InVolume;
 
	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld,
		DefaultSoundMix,
		MasterSoundClass,
		OverallVolume,
		1.f,
		0.2f
	);
 
	UGameplayStatics::PushSoundMixModifier(InAudioWorld, DefaultSoundMix);
}

void UFrontendGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
}

void UFrontendGameUserSettings::SetSoundVFXVolume(float InVolume)
{
	SoundVFXVolume = InVolume;
}

void UFrontendGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
}

void UFrontendGameUserSettings::SetUseHDRAudio(bool bIsUseHDR)
{
	bUseHDRAudio = bIsUseHDR;
}

float UFrontendGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 2.2f;
}

void UFrontendGameUserSettings::SetCurrentDisplayGamma(float InCurrentDisplayGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InCurrentDisplayGamma;
	}
}
