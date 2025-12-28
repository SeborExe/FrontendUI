// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

UCLASS()
class FRONTUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UFrontendGameUserSettings();

	static UFrontendGameUserSettings* Get();

	//Gameplay collection tab
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }
	UFUNCTION()
	void SetCurrentGameDifficulty(FString InGameDifficulty) { CurrentGameDifficulty = InGameDifficulty; }
	//Gameplay collection tab

	//Gameplay Audio tab
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION()
	void SetOverallVolume(float InVolume);
	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION()
	void SetMusicVolume(float InVolume);
	UFUNCTION()
	float GetSoundVFXVolume() const { return SoundVFXVolume; }
	UFUNCTION()
	void SetSoundVFXVolume(float InVolume);
	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }
	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);
	UFUNCTION()
	bool GetUseHDRAudio() const { return bUseHDRAudio; }
	UFUNCTION()
	void SetUseHDRAudio(bool bIsUseHDR);
	//Gameplay Audio tab

	//Vide Collection Tab
	UFUNCTION()
	float GetCurrentDisplayGamma() const;
	UFUNCTION()
	void SetCurrentDisplayGamma(float InCurrentDisplayGamma);
	//Vide Collection Tab

	
private:

	// GAMEPLAY TAB
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	// GAMEPLAY TAB

	// AUDIO TAB
	UPROPERTY(Config)
	float OverallVolume;
	UPROPERTY(Config)
	float MusicVolume;
	UPROPERTY(Config)
	float SoundVFXVolume;
	UPROPERTY(Config)
	bool bAllowBackgroundAudio;
	UPROPERTY(Config)
	bool bUseHDRAudio;
	// AUDIO TAB

	// VIDEO TAB
	
	// VIDEO TAB
};
