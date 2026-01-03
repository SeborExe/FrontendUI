// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FrontendLoadingScreenInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UFrontendLoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

class FRONTUI_API IFrontendLoadingScreenInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();
	//virtual void OnLoadingScreenActivated_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
};
