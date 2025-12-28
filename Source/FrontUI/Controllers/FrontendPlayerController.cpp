// Fill out your copyright notice in the Description page of Project Settings.

#include "FrontUI/Controllers/FrontendPlayerController.h"

#include "Camera/CameraActor.h"
#include "FrontUI/DeveloperSettings/FrontendGameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void AFrontendPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		SetViewTarget(FoundCameras[0]);
	}

	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();
	if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		GameUserSettings->RunHardwareBenchmark();
		GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
