// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontUI/Widgets/Options/DataObjects/MyListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

UCLASS()
class FRONTUI_API UListDataObject_StringResolution : public UMyListDataObject_String
{
	GENERATED_BODY()

public:

	void InitResolutionValues();

protected:

	virtual void OnDataObjectInitialized() override;

private:

	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;

public:

	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }
};
